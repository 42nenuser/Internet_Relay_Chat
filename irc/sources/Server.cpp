#include "Server.hpp"

/**
 * @brief Constructs a Server object with the specified port and password.
 *
 * @param port The port number to listen on.
 * @param password The password required to connect to the server.
 */
Server::Server(int port, std::string password) {
	this->server = new server_info;
	this->server->password = password;
	this->server->port = port;
	try {
		ConnectionManager* connectionManager = new ConnectionManager(this->server);
		PollManager* pollManager = new PollManager(server, pollfds, users, requests, this->channels);
		delete connectionManager;
		delete pollManager;
		close_server();
	} catch (std::exception& e) {
		std::cout << BOLD BRIGHT_RED << e.what() << RESET << std::endl;
		close_server();
	}
}

Server::~Server() {}

/**
 * @brief Closes the server and performs cleanup before quitting.
 * 
 * This function closes all file descriptors, deletes all users, requests, and channels,
 * and then exits the program with a success status.
 */
void	Server::close_server(void) {
	std::vector<struct pollfd>::iterator it = pollfds.begin();
	std::cout << BOLD BRIGHT_RED << "[CLEANUP] " << RESET << "cleaning server before quitting." << std::endl;
	for (; it != pollfds.end(); it++) {
		close((*it).fd);
	}
	while (users.size() > 0) {
		User* user = users.begin()->second;
		users.erase(users.begin());
		delete(user);
	}
	while (requests.size() > 0) {
		Request* req = requests.front();
		requests.pop();
		delete req;
	}
	while (channels.size() > 0) {
		Channel* channel = channels.begin()->second;
		channels.erase(channels.begin());
		delete channel;
	}
	delete server;
	exit(EXIT_SUCCESS);
}
