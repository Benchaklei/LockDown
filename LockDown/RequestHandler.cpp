#include "RequestHandler.h"

/*
Checks if the request had a valid msg code
input: msg
output: true/false
*/
bool RequestHandler::isRequestRelevant()
{
	if (!(SIGN_UP_CODE <= this->_msg_code  && this->_msg_code <= SEND_KEY)) return false; // the request is not relevant
	return true;
}

/*
Handles the request of the user, sending the correct response to the user
input: string - user's msg
output: the server's response according to the requesst
*/
std::string RequestHandler::handleRequest(std::string data)
{
	std::string res = "";

	if (this->_msg_code == SIGN_UP_CODE)
	{
		json parsed_data = json::parse(data.begin(), data.end());
		std::string name = parsed_data["username"], password = parsed_data["password"];
		std::string db_response = data_base.addUserToDb(name, password);
		res = std::to_string(SIGN_UP_RESPONSE) + db_response;
	}
	else if (this->_msg_code == LOGIN_CODE)
	{
		json parsed_data = json::parse(data.begin(), data.end());
		std::string name = parsed_data["username"], password = parsed_data["password"];
		std::string hasLogged = "";
		std::string loggedSuccess = std::to_string(LOGIN_RESPONSE);
		if (data_base.doesUserExist(name, password)) {
			hasLogged = "Logged In!\n";
			_userName = name;
		}
		else {
			_userName = "";
			loggedSuccess = "";
			hasLogged = "User/Password is wrong\n";
		}
		res = loggedSuccess + hasLogged;
	}
	else if (this->_msg_code == SAVE_KEY)
	{
		std::string keyEnc = key_generator::create_key();
		std::string iv_key = key_generator::create_key();
		data_base.addKeyToDb(_userName, keyEnc, iv_key);
		res = std::to_string(SAVE_KEY_RESPONSE) + keyEnc + iv_key;
	}
	else if (this->_msg_code == SEND_KEY)
	{
		std::vector<std::string> keyAndIvDec = data_base.getKeyFromUser(_userName);
		data_base.deleteKey(_userName);
		res = std::to_string(SEND_KEY_RESPONSE) + keyAndIvDec[0] + keyAndIvDec[1];
	}

	return res;
}

void RequestHandler::setMsgCode(int msgCode)
{
	this->_msg_code = msgCode;
}

std::string RequestHandler::getUserName() {
	return _userName;
}

int RequestHandler::getMsgCode()
{
	return this->_msg_code;
}
