README for Socket Client and Server
Original author: Morgan McKinney 11/2020

Basic TCP/IP client and server network that allows the client to upload files to the server. Application allows for socket connections to take place over a network other than 'localhost'.

server.cpp: Source code file for server. Creates a socket, binds, listens, accepts any connecting clients, saving any text files that have been uploaded to a file called server.txt. After 5 minutes, the server will disconnect itself to avoid an infinite loop. Outputs when the server starts and ends, client connects and disconnects, and when a file is recieved.

client.cpp: Source code file for client. Creates a socket, connects to the server, uploads a text file, then disconnects. Outputs when it connects to the server, sends the file, and ends the connection.

server.txt: Output of server.cpp, which is uploaded text from the client application.

hello.txt: Sample text file I used to test my application. Reads "hello world".

server: Executable of server.cpp. If not functional, compile using instructions below.

client: Executable of client.cpp. If not functional, compile using instructions below.

How to test and run application:
First, please compile the server and client source code files separately by entering...
> g++ -o server server.cpp
> g++ -o client client.cpp
Then, open up two separate terminals. Run the server executable first by entering...
> ./server
Next, in your other terminal, run the client executable with the necessary parameters...
> ./client IPAddr fileLocation
Where IPAddr is the network you want the socket connections to take place over.
When I tested this application myself, I tested on the 'localhost' network (127.0.0.1).
Then, the next parameter is fileLocation which is the path of the file you wish to upload.
When I tested this application myself, I uploaded a file in the same folder named 'hello.txt'.
Thus, a sample input that I tested myself is...
> ./client 127.0.0.1 ~/CS371/Project/hello.txt

References:
Christian Tooley's socket tutorial video on YouTube
http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
https://www.geeksforgeeks.org/socket-programming-cc/
http://www.cplusplus.com/doc/tutorial/ntcs/
https://stackoverflow.com/questions/25604885/how-to-convert-a-string-to-const-void-in-c
