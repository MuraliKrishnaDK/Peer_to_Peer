# Peer_to_Peer

#Objective

Creation of a client and a server application for this project that allows client-to-client communication but not server-to-client communication. The server application will only be used to exchange contact details for clients who are currently active. Clients can connect to the server, log in with a password, and view the list of clients who are currently active. Any active client can be selected as the connection point, and the remote client will be prompted to accept the new connection. Once a connection has been established successfully, clients can communicate with one another independently of the server. Additionally, a client can ask the server to send a message to every client in its database by making a broadcast request. The broadcast message should be received by all clients who will be online in the next 30 minutes.

#Features
1. Brodcasting message
2. Peer to peer messaging
3. Login and SignUp
4. List of active clients
5. Hashing Passwords
6. Login request every 30mins

#Prerequisities
1. C++ compiler
2. Importing Libraries

#Execution
1. Route the terminal to the location where code is stored
2. Server code Compilation in the terminal using the command - g++ server.cpp -o server pthread
3. Give the command to run the server code - ./server
4. The server begins to listen for client request 
5. Open a new terminal to compile the client code using command - g++ client.cpp -o client pthread
6. Run the client code with the command - ./client

With the client code executed, the following images depicts the features of the project -

https://github.com/MuraliKrishnaDK/Peer_to_Peer/blob/main/server.jpeg
https://github.com/MuraliKrishnaDK/Peer_to_Peer/blob/main/murali_signup.jpeg
https://github.com/MuraliKrishnaDK/Peer_to_Peer/blob/main/murali_broadcast.jpeg
https://github.com/MuraliKrishnaDK/Peer_to_Peer/blob/main/sai_signup.jpeg
https://github.com/MuraliKrishnaDK/Peer_to_Peer/blob/main/sai_broadcast.jpeg
https://github.com/MuraliKrishnaDK/Peer_to_Peer/blob/main/ravi_signup.jpeg
https://github.com/MuraliKrishnaDK/Peer_to_Peer/blob/main/ravi_clientlist.jpeg
