// ======================  Libraries =====================


#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <chrono>
#include <ctime>
#include <netdb.h>
#include <map>
#include <thread>
#include <fstream>
#include <vector>
#include <pthread.h>
#include <vector>
 

int total=0;

// Define maximum number of connections
#define MAX_CONNECTIONS 50

// Define message buffer size
#define BUFFER_SIZE 1024

// Define broadcast time limit in seconds
#define BROADCAST_TIME_LIMIT 1800
std::vector<std::thread> threads;
std::vector<int> Num_clients;


// Define client information struct
struct client_info {
    std::string username;
    std::string password;
    std::string ip_address;
    int tcp_socket;
};

std::vector<client_info> clients;

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// Declare UDP socket descriptor
int udp_socket;

// Declare function prototypes
void handle_new_client(int client_socket);

std::vector<std::string>IP_Retrieve()
{
char hostbuffer[256];
std::vector<std::string> g1;
	struct hostent *host_entry;
	int hostname;
	struct in_addr **addr_list;
	sockaddr_in addr {};
	socklen_t len = sizeof(addr);
	hostname = gethostname(hostbuffer, sizeof(hostbuffer));
	host_entry = gethostbyname(hostbuffer);
	if (host_entry == NULL) {
		perror("gethostbyname error");
		exit(1);
	}
	addr_list = (struct in_addr **)host_entry->h_addr_list;
	        for(int i=0;i<total;i++){
		 g1.push_back("Client "+ std::to_string(i+1) + ":" + inet_ntoa(*addr_list[0]));
		}
		std::cout << "Port number: " << ntohs(addr.sin_port) << std::endl;
return g1;
}
void accept_clients(int tcp_socket) {
    while (true) {
        int client_socket, client_addr_len;
        struct sockaddr_in client_address;

        // Accept new client connection
        client_addr_len = sizeof(client_address);
        if ((client_socket = accept(tcp_socket, (struct sockaddr*)&client_address, (socklen_t*)&client_addr_len)) < 0) {
            std::cerr << "Error accepting new connection." << std::endl;
            continue;
        }
	std::cout << "Port number: " << ntohs(client_address.sin_port) << ":"<<inet_ntoa(client_address.sin_addr)<<std::endl;
        // Create a new thread to handle the new client connection
        std::thread client_thread(handle_new_client, client_socket);
        client_thread.detach(); // detach thread to run independently
        Num_clients.push_back(client_socket);
    }
}
using namespace std;
int main() {
    int tcp_socket;
    struct sockaddr_in tcp_address;
     //inet_ntoa(*addr_list[0])
    // Create TCP socket
    if ((tcp_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Error creating TCP socket." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Bind socket to address
    tcp_address.sin_family = AF_INET;
    tcp_address.sin_addr.s_addr = htonl(INADDR_ANY);
    tcp_address.sin_port = htons(6405); // use port 8080
    if (bind(tcp_socket, (struct sockaddr*)&tcp_address, sizeof(tcp_address)) < 0) {
        std::cerr << "Error binding TCP socket." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(tcp_socket, MAX_CONNECTIONS) < 0) {
        std::cerr << "Error listening for connections." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Accept incoming connections in separate threads
    
    
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        threads.push_back(std::thread(accept_clients, tcp_socket));
    }

    // Join threads (never reached in this example)
    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
void handle_new_client(int client_socket) {
    char buffer[1024];
    int bytes_received = recv(client_socket, buffer, 1024, 0);
    std::string request(buffer, bytes_received);

    // Respond with login/signup prompt
    std::string response = "Welcome! Do you want to: login or signup\n";
    send(client_socket, response.c_str(), response.length(), 0);

    while (true) {
        bytes_received = recv(client_socket, buffer, 1024, 0);
        // if (bytes_received < 0) {
        //     // error handling
        //     return;
        // }
        std::string choice(buffer, bytes_received);

        if (choice == "login") {
            // Ask for username and password
            response = "Please enter your username:\n";
            send(client_socket, response.c_str(), response.length(), 0);

            bytes_received = recv(client_socket, buffer, 1024, 0);
            if (bytes_received < 0) {
                // error handling
                return;
            }
            std::string username(buffer, bytes_received);

            response = "Please enter your password:\n";
            send(client_socket, response.c_str(), response.length(), 0);

            bytes_received = recv(client_socket, buffer, 1024, 0);
            if (bytes_received < 0) {
                // error handling
                return;
            }
            std::string password(buffer, bytes_received);

            // TODO: Check authentication and handle accordingly
             // Check authentication from CSV file
            std::ifstream file("users.csv");
            std::string line;
            bool authenticated = false;
            while (std::getline(file, line)) {
                size_t pos = line.find(',');
                if (pos != std::string::npos) {
                    std::string file_username = line.substr(0, pos);
                    std::string file_password = line.substr(pos+1);
                    if (file_username == username && file_password == password) {
                        authenticated = true;
                        break;
                    }
                }
            }
            file.close();

            if (authenticated) {
                total++;
                response = "Successfully logged in!\n";
                
                send(client_socket, response.c_str(), response.length(), 0);
                
                 // Show options to the authenticated client
        response = "You are authenticated. Select any option:\n1. List of active clients\n2. Broadcast a message\n";
        send(client_socket, response.c_str(), response.length(), 0);

        bytes_received = recv(client_socket, buffer, 1024, 0);
        if (bytes_received < 0) {
            // error handling
            return;
        }
        std::string option(buffer, bytes_received);

        if (option == "1") {
           vector<string> a=IP_Retrieve();
      
           
            // List active clients
            // Assume active_clients is a vector of connected clients with their username and IP address
              //vector<string> active_clients = get_active_clients();
            std::string active_clients_list = "Active Clients chatting:\n";
            for (const auto& client : a) {
                active_clients_list +=   client +"\n";
            }
            send(client_socket, active_clients_list.c_str(), active_clients_list.length(), 0);
            bytes_received = recv(client_socket, buffer, 1024, 0);
        if (bytes_received < 0) {
            // error handling
            return;
        }
        std::string ipadd(buffer, bytes_received);
        

        } else if (option == "2") {
            // Broadcast a message
            response = "Enter the message to broadcast:\n";
            send(client_socket, response.c_str(), response.length(), 0);

            bytes_received = recv(client_socket, buffer, 1024, 0);
            if (bytes_received < 0) {
                // error handling
                return;
            }
            std::string msg(buffer, bytes_received);
            std::string broadcast_message = "Message is broadcasted by " + std::to_string(client_socket-2) + "\nMessage ::\n"+msg; 
             for (int client_socket : Num_clients) {
                    std::cout << " client number is : " <<  client_socket << "\n";
            send(client_socket, broadcast_message.c_str(), broadcast_message.size(), 0);
                }
            // Append sender's information and date/time to the broadcast message
            //std::string complete_broadcast_message = "Broadcast from " + username + " at " + get_current_datetime() + ": " + broadcast_message + "\n";


            response = "Message broadcasted successfully!\n";
            send(client_socket,response.c_str(), response.length(), 0);
} 
else if (option == "3") {
            //peer_to_peer_communicationelse()
            }else{
// Invalid option, terminate client's connection
response = "Invalid option. Terminating connection...\n";
send(client_socket, response.c_str(), response.length(), 0);
close(client_socket);
return;
}

            } else {
                response = "Login failed. Invalid username or password.\n";
                send(client_socket, response.c_str(), response.length(), 0);
            }
        } 


            else if (choice == "signup") {
            // Ask for new username and password
            response = "Please enter a new username:\n";
            send(client_socket, response.c_str(), response.length(), 0);

            bytes_received = recv(client_socket, buffer, 1024, 0);
            if (bytes_received < 0) {
                // error handling
                return;
            }
            std::string new_username(buffer, bytes_received);

            // Check if username already exists in CSV file
            std::ifstream file("users.csv");
            std::string line;
            bool username_exists = false;
            while (std::getline(file, line)) {
                size_t pos = line.find(',');
                if (pos != std::string::npos) {
                std::string username_from_file = line.substr(0, pos);
                if (new_username == username_from_file) {
                username_exists = true;
                    break;
                    }
                    }
                        }
                file.close();

                 if (username_exists) {
            response = "Username already exists. Please choose another username.\n";
            send(client_socket, response.c_str(), response.length(), 0);
        } else {
            response = "Please enter a new password:\n";
            send(client_socket, response.c_str(), response.length(), 0);

            bytes_received = recv(client_socket, buffer, 1024, 0);
            if (bytes_received < 0) {
                // error handling
                return;
            }
            std::string new_password(buffer, bytes_received);

            response = "Please confirm your password:\n";
            send(client_socket, response.c_str(), response.length(), 0);

            bytes_received = recv(client_socket, buffer, 1024, 0);
            if (bytes_received < 0) {
                // error handling
                return;
            }
            std::string confirmed_password(buffer, bytes_received);

            if (new_password != confirmed_password) {
                response = "Passwords do not match. Please try again.\n";
                send(client_socket, response.c_str(), response.length(), 0);
            } else {
                // Write username and password to CSV file
                std::ofstream outfile;
                outfile.open("users.csv", std::ios_base::app);
                outfile << new_username << "," << confirmed_password << std::endl;
                outfile.close();

                response = "Signup successful. Please login to continue.\n";
                send(client_socket, response.c_str(), response.length(), 0);
            }
        }

        } else {
            // Invalid choice
            response = "Invalid option. Please choose login or signup\n";
            send(client_socket, response.c_str(), response.length(), 0);
        }
    }
}
