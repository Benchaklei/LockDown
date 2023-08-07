#pragma once

#include <string>
#include <pqxx/pqxx>
#include <iostream>
#include <string>
#include <vector>
using namespace pqxx;

class DB_Manager 
{
public:
	connection* conn;

	DB_Manager();

	//int openDataBase();
	void setConnection();
	void disconnect();

	bool doesUserExist(std::string name, std::string password);
	std::string addUserToDb(std::string name, std::string password);

	std::string addKeyToDb(std::string name, std::string key, std::string iv);
	std::vector<std::string> getKeyFromUser(std::string name);

	std::string deleteUser(std::string name, std::string password);
	std::string deleteKey(std::string name);
	
private:
	const std::string dbName = "lc_db";
};