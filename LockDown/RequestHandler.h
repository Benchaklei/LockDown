#pragma once

#include "key_generator.h"
#include "DB_Manager.h"
#include "json.hpp"

using nlohmann::json;

enum msg_codes
{
	SIGN_UP_CODE = 101,
	LOGIN_CODE = 102,
	SAVE_KEY = 103,
	SEND_KEY = 104,

	SIGN_UP_RESPONSE = 201,
	LOGIN_RESPONSE = 202,
	SAVE_KEY_RESPONSE = 203,
	SEND_KEY_RESPONSE = 204,
};

class RequestHandler 
{
public:
	bool isRequestRelevant();
	std::string handleRequest(std::string data);
	void setMsgCode(int msgCode);
	std::string getUserName();
	int getMsgCode();

private:
	std::string _userName = "";
	int _msg_code;
	DB_Manager data_base;
};