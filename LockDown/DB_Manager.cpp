#include "DB_Manager.h"

// Connect to the psql database
void DB_Manager::setConnection() 
{
	try
	{
		this->conn = new connection("user=postgres password=beklagilga host=localhost port=42070 dbname=lc_db");
		std::cout << "Connected to db!" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

// Disconnected from the database
void DB_Manager::disconnect() 
{
	if (this->conn == nullptr)
	{
		std::cout << "ERROR: can't disconnect from database, no database was connected." << std::endl;
	}
	else
	{
		this->conn->close();
		std::cout << "Disconnected from db." << std::endl;
	}
}


/* 
Checks if the user exists
input: name and password
output: yes/true(based on condition if the user exists=
*/
bool DB_Manager::doesUserExist(std::string name, std::string password) 
{
	std::string query = "SELECT FROM USERS WHERE USERNAME='" + name + "' AND USER_PASSWORD='" + password + "';";
	
	nontransaction N(*this->conn);
	result query_Res(N.exec(query));

	if (query_Res.size() == 1) 
		return true;
	else 
		return false;
}

/* 
Adds a user to the DataBase
input: name and password
output: none
*/
std::string DB_Manager::addUserToDb(std::string name, std::string password) 
{
	try
	{
		work W(*this->conn);

		std::string query = "INSERT INTO USERS(USERNAME,USER_PASSWORD) VALUES('" + name + "','" + password + "');";

		W.exec(query);
		W.commit();
		return "Added Successfully\n";
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return "Error Occured!\n";
	}
}

/*
Adds a key to a user in the database
input: name - name of user, key - user's key to insert
output: none
*/
std::string DB_Manager::addKeyToDb(std::string name, std::string key, std::string iv)
{
	try
	{
		work W(*this->conn);

		std::string query = "INSERT INTO KEYS(USER_NAME,KEY_STR,IV_STR) VALUES(" +
			W.quote(name) + ", " +
			W.quote(key) + ", " +
			W.quote(iv) + ")";

		W.exec(query);
		W.commit();
		return "Added Successfully\n";
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return "Error Occured!\n";
	}
}

/*
Gets a user's key from DB
input: user name
output: string - user's key
*/
std::vector<std::string> DB_Manager::getKeyFromUser(std::string name)
{
	std::vector<std::string> toRet;
	try
	{
		std::string query = "SELECT key_str,iv_str FROM KEYS WHERE user_name = $1"; // $1 is a parameter which we will insert in the name in

		nontransaction txn(*this->conn); // this way we save the query
		result res(txn.exec_params(query, name)); // we save the result

		if (res.size() > 0) 
		{
			toRet.push_back( res[0]["key_str"].as<std::string>());
			toRet.push_back(res[0]["iv_str"].as<std::string>());
		}
		else 
		{
			std::cerr << "Username/Key does not exist in the database." << std::endl;
			toRet.push_back("Username/Key does not exist!\n");
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return toRet;
}

/*
Deletes a user with a specific name and password
input: name and password of user to delete
output: none
*/
std::string DB_Manager::deleteUser(std::string name, std::string password) 
{
	if (!doesUserExist(name, password)) 
	{
		std::cout << "user does not exist" << std::endl;
		return "User does not exist!\n";
	}

	std::string query = "DELETE FROM USERS WHERE USERNAME='" + name + "' AND USER_PASSWORD='" + password + "';";
	
	try 
	{
		work W(*this->conn);
		W.exec(query);
		W.commit();
		return "Deleted Successfully\n";
	}
	catch (const std::exception& e) 
	{
		std::cout << e.what() << std::endl;
	}
	return "Error Occured\n";
}

/*
Delets a key from DataBase
input: name of user
output: none
*/
std::string DB_Manager::deleteKey(std::string name) 
{
	if (getKeyFromUser(name).empty()) 
	{
		return "Error Occured!\n";
	}

	std::string query = "DELETE FROM KEYS WHERE USER_NAME='" + name + "';";

	try 
	{
		work W(*this->conn);
		W.exec(query);
		W.commit();
		return "Added successfully\n";
	}
	catch (const std::exception& e) 
	{
		std::cout << e.what() << std::endl;
	}
	return "Error Occured!\n";
}

DB_Manager::DB_Manager() {
	this->setConnection();
}