#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include "RequestHandler.h"
#include <exception>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include "DB_Manager.h"

#pragma comment(lib, "Ws2_32.lib")

#define SERVER_PORT 42069
#define MSG_TYPE_LEN 3
#define RECIEVE_ERROR -1
#define MSG_SIZE 200

class Server
{
public:
	Server();
	~Server();
	void serve(int port = SERVER_PORT);

private:
	void acceptClient();
	void clientHandler(SOCKET clientSocket);
	SOCKET _serverSocket;
	std::vector<std::string> usersLogged;
};


