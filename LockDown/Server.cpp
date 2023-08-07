#include "Server.h"

/*
The server has 4 types of queries:
- Start Encryption - Save encryption key on DB
- Start Decryption - Give encryption key from DB
- Login (user name + password)
- Signup (user name + password)

* Note that LOGIN is needed in order to proceed to encryption/decryption !!

*/

Server::Server()
{
	// we need to use this to set up the Winsock API and return a value indicating whether the initialization was successful or not
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		throw std::exception(__FUNCTION__ " - WSAStartup failed");

	// this server use TCP. that why SOCK_STREAM & IPPROTO_TCP
	// if the server use UDP we will use: SOCK_DGRAM & IPPROTO_UDP
	_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (_serverSocket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - socket");
}

Server::~Server()
{
	try
	{
		// the only use of the destructor should be for freeing 
		// resources that was allocated in the constructor
		closesocket(_serverSocket);
		WSACleanup(); // release the resources allocated by WSAStartup
	}
	catch (...) {}
}


void Server::serve(int port)
{
	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(port); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY;    // when there are few ip's for the machine. We will use always "INADDR_ANY"

	// Connects between the socket and the configuration (port and etc..)
	if (bind(_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");

	// Start listening for incoming requests of clients
	if (listen(_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	std::cout << "Listening on port " << port << std::endl;

	while (true)
	{
		// the main thread is only accepting clients 
		// and add then to the list of handlers
		std::cout << "Waiting for client connection request" << std::endl;
		acceptClient();
	}
}

// THIS AND THE FUNCTIONS ABOVE WILL BE FUNCTION RUNNING BY THE CONNECTOR THREAD
void Server::acceptClient()
{
	// this accepts the client and create a specific socket from server to this client
	// the process will not continue until a client connects to the server
	SOCKET client_socket = accept(_serverSocket, NULL, NULL);
	if (client_socket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__);

	std::cout << "Client accepted. Server and client can speak" << std::endl;
	// the function that handle the conversation with the client
	std::thread clientThread(&Server::clientHandler, this, client_socket); // THE CONNECTOR THREAD SENDS THE SOCKET TO A NEW CLIENT THREAD
	clientThread.detach();
}

// THIS WILL BE THE FUNCTION THE CLIENT THREAD WILL RUN
void Server::clientHandler(SOCKET clientSocket)
{
	RequestHandler* reqHandler = new RequestHandler;
	std::string errorMsg = "", msg = "", data = "", handlerUserName = "", error = "";
	char buffer[MSG_SIZE] = { 0 };
	bool userDisconnected = false;

	while (!userDisconnected) 
	{
		try
		{
			int res = recv(clientSocket, buffer, MSG_SIZE, 0); 
			if (res == RECIEVE_ERROR) { // couldn't receieve data from user socket
				error = "User: " + handlerUserName + " Has Disconnected\n";
				throw std::exception(error.c_str());
			}
			msg = std::string(buffer);
			reqHandler->setMsgCode(stoi(msg.substr(0, MSG_TYPE_LEN))); // get the msg code of the user msg
			data = msg.substr(3, msg.size()); // get the data of the user msg
			if (reqHandler->isRequestRelevant()) 
			{
				std::string res = reqHandler->handleRequest(data);
				handlerUserName = reqHandler->getUserName(); // get the user name of the handler
				if (reqHandler->getMsgCode() == LOGIN_CODE && std::find(usersLogged.begin(), usersLogged.end(), handlerUserName) == usersLogged.end()) {
					usersLogged.push_back(handlerUserName); // add user if does not exist
				}
				else if (std::find(usersLogged.begin(), usersLogged.end(), handlerUserName) != usersLogged.end() && reqHandler->getMsgCode() == LOGIN_CODE) {
					res = "User is already logged in!\n";
				}
				send(clientSocket, res.c_str(), res.size(), 0);
			}
			else
			{
				errorMsg = "Invalid Request Type!";
				send(clientSocket, errorMsg.c_str(), errorMsg.size(), 0);
				throw std::exception(errorMsg.c_str());
			}

		}
		catch (const std::exception& e) // close user socket and remove it from the logged users' vector
		{
			userDisconnected = true;
			closesocket(clientSocket);
			auto doesExist = std::find(usersLogged.begin(), usersLogged.end(), handlerUserName);
			if (doesExist != usersLogged.end()) {
				usersLogged.erase(doesExist);
			}
			std::cout << e.what() << std::endl;
		}
	}
	delete reqHandler; // remove the handler from memory
}

