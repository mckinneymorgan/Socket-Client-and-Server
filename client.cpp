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
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

int main(int argc, char* argv[])
{
	// Grab port
	int port = 25434;
	if(argc != 3) // Check for correct parameters
	{
		cout << "Usage: ./client IPAddr fileLocation" << endl;
		return -1; // Exit
	}
	char* IPAddr = argv[1]; // IP address
	string fileLocation = argv[2]; // Location of file to be transfered

	// Check that file path is valid
	ifstream file;
	file.open(fileLocation);
	if(!file) // If unable to open file, exit
	{
		cout << "File does not exist/invalid path" << endl;
		return -1; // Exit
	}

	// Set up client socket
	sockaddr_in clientAddr;
	// Tell socket to use IPV4
	clientAddr.sin_family = AF_INET;
	// Make sure port is stored correctly, Big Endian vs. Little Endian
	clientAddr.sin_port = htons(port);

	// Create socket:
	// AF_INET stands for IPV4
	// SOCK_STREAM stands for TCP instead of UDP
	// 0 is our protocol value for IP
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket == 0) // If socket fails to create, exit
	{
		perror("Client socket creation failed");
		exit(EXIT_FAILURE);
	}

	// Convert IPV4 address to binary
	int addrConversionStatus = inet_pton(AF_INET, IPAddr, &clientAddr.sin_addr);
	if(addrConversionStatus <= 0) // If unable to convert IPAddr, exit
	{
		perror("Invalid address");
		exit(EXIT_FAILURE);
	}

	// Connect to server
	int connectionStatus = connect(clientSocket, (struct sockaddr*)&clientAddr, sizeof(clientAddr));
	if(connectionStatus < 0) // If client fails to connect, exit
	{
		perror("Client failed to connect");
		exit(EXIT_FAILURE);
	}

	// Output that client has connected to server
	cout << "Connected to server..." << endl;

	// Send file to server
	string line;
	while (getline(file,line)) // Read file line by line
	{
		const void* lineConverted = line.c_str();
		send(clientSocket, lineConverted, line.length(), 0);
	}
	cout << "File sent..." << endl;

	// Close file and socket
	close(clientSocket);
	file.close();

	// Output that connection has ended
	cout << "Connection ended..." << endl;

	return 0;
}
