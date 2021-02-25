// Original author: Morgan McKinney 11/2020

#include <sys/types.h>
#include <stdio.h>
#include <fstream>
#include <fcntl.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[])
{
	// Grab port
	int port = 25434;
	char message[1000];

	// Set up server socket
	sockaddr_in servAddr;
	memset((char*)&servAddr, 0, sizeof(servAddr));
	// Tell socket to use IPV4
	servAddr.sin_family = AF_INET;
	// Binds our socket to all IP's, not just localhost, i.e. can connect to anyone
	servAddr.sin_addr.s_addr = INADDR_ANY;
	// Make sure port is stored correctly, Big Endian vs. Little Endian
	servAddr.sin_port = htons(port);

	// Create socket:
	// AF_INET stands for IPV4
	// SOCK_STREAM stands for TCP instead of UDP
	// 0 is our protocol value for IP
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(serverSocket == 0) // If socket fails to create, exit
	{
		perror("Server socket creation failed");
		exit(EXIT_FAILURE);
	}

	// Bind socket to port
	int bindStatus = bind(serverSocket, (struct sockaddr*)&servAddr, sizeof(servAddr));
	if(bindStatus < 0) // If binding fails, exit
	{
		perror("Server socket bind failed");
		exit(EXIT_FAILURE);
	}

	// Listen for client
	int listenStatus = listen(serverSocket, 5);
	if(listenStatus < 0) // If listening fails, exit
	{
		perror("Server failed to listen");
		exit(EXIT_FAILURE);
	}

	// Output that server has started
	cout << "Server started..." << endl;

	// Set up new socket
	sockaddr_in newSockAddr;
	socklen_t newSockAddrSize = sizeof(newSockAddr);

	// Accept connection
	int newSd = accept(serverSocket, (sockaddr*)&newSockAddr, &newSockAddrSize);
	if(newSd < 0) // If server fails to accept connection, exit
	{
		perror("Server failed to accept");
		exit(EXIT_FAILURE);
	}

	// Output that client has connected
	cout << "Client connected..." << endl;

	// Initialize variables
	bool connection = true; // Inidicates if there is a current connection
	int serverRuntime = 300; // Seconds server will run before closing sockets
	int recvValue; // Value returned by reading recv()
	ofstream file; // Create file to store uploaded text(s)
	file.open("server.txt"); 

	while(serverRuntime > 0) // Once client has connected, wait for a message
	{	
		memset(&message, 0, sizeof(message));
		recvValue = recv(newSd, (char*)&message, sizeof(message), 0);
		
		// Detect if client has disconnected
		if(recvValue <= 0 && connection == true)
		{
			// Output that connection has ended
			cout << "Connection ended..." << endl;
			connection = false;
		}
		// Detect that another client has connected
		else if(recvValue > 0 && connection == false)
		{
			// Output that client has connected
			cout << "Client connected..." << endl;
			connection = true;
		}

		// Detect if file has been recieved
		if(message[0] != '\0')
		{
			file << message << endl;
			// Output that file has been recieved
			cout << "File recieved..." << endl;
		}

		sleep(1); // Wait 1 second
		serverRuntime--; // Decrement runtime timer
	}

	// Clean up sockets and close file, once finished
	close(newSd);
	close(serverSocket);
	file.close();

	// Output that server has ended
	cout << "Server ended..." << endl;

	return 0;
}
