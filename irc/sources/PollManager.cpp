#include "PollManager.hpp"
#include "Color.h"
#include "QuitCmd.hpp"

/**
 * Handles a new connection from a client.
 * 
 * This function accepts a new connection from a client, sets the client socket to non-blocking mode,
 * creates a User object for the client, and adds the client to the list of pollfds.
 * 
 * @return void
 */
void PollManager::handle_new_connection() {
	struct pollfd client;
	client.fd = accept(server->fd, NULL, NULL);
	if (client.fd == -1) {
		std::cout << BOLD BRIGHT_YELLOW << "[PMWARN] " << RESET << "server failed to accept new connection" << RESET << std::endl;
		return;
	}
	if (fcntl(client.fd, F_SETFL, O_NONBLOCK) == -1) {
		std::cout << BOLD BRIGHT_YELLOW << "[PMWARN]" << RESET << " couldn't set client socket to non-blocking mode" << std::endl;
		std::cout << BOLD BRIGHT_YELLOW << "[PMWARN]" << RESET << " closing connection with client : " << client.fd << std::endl;
		close(client.fd);
		return ;
	}
	std::cout << BOLD BRIGHT_PURPLE << "[PMINFO] " << RESET << "new connection from client(" << client.fd << ")" << RESET <<std::endl;
	User* new_user = new User(client.fd);
	this->users->insert(std::make_pair(client.fd, new_user));
	std::cout << BOLD BRIGHT_PURPLE << "[PMINFO]" << RESET << " creating a User for client(" << client.fd << ")" << std::endl;
	client.events = POLLIN | POLLOUT;
	this->pollfds->push_back(client);
}

/**
 * @brief Removes a disconnected client from the poll manager.
 * 
 * This function is responsible for removing a disconnected client from the poll manager.
 * It takes the index of the client in the pollfds array and performs the necessary cleanup operations.
 * The client's file descriptor is retrieved from the pollfds array and used to create a new Request object.
 * The Request object is then configured with the "QUIT" command and a message indicating that the client is leaving.
 * Finally, the Request object is added to the requests queue for further processing.
 * 
 * @param index The index of the disconnected client in the pollfds array.
 */
void	PollManager::remove_disconnected_client(size_t index)
{
	int client_fd = (*pollfds)[index].fd;
	Request* new_request = new Request(client_fd);
	new_request->set_cmd("QUIT");
	new_request->set_options("Leaving...");
	requests->push(new_request);
}

/**
 * @brief Handles client activity for a specific index in the pollfds array.
 * 
 * This function receives data from the client socket and performs the necessary actions based on the received data.
 * If an error occurs while receiving data, a warning message is printed.
 * If the client socket is disconnected, the client is removed from the pollfds array.
 * If a complete message is received, it is processed by the IRC parser.
 * 
 * @param index The index of the client in the pollfds array.
 */
void 	PollManager::handle_client_activity(size_t index) {
	char buffer[BUFSIZ];
	int byte_read;

	memset(buffer, '\0', BUFSIZ);
	byte_read = recv((*pollfds)[index].fd, buffer, BUFSIZ, 0);
	if (byte_read == -1)
		std::cout << BOLD BRIGHT_YELLOW << "[PMWARN] " << RESET << "error while receiving data from client with fd : " << (*pollfds)[index].fd << std::endl;
	else if (byte_read == 0)
		remove_disconnected_client(index);
	else {
		partial_data[(*pollfds)[index].fd] += buffer;

		size_t pos = partial_data[(*pollfds)[index].fd].rfind('\n');
		if (pos != std::string::npos) {
			std::string complete_data = partial_data[(*pollfds)[index].fd].substr(0, pos + 1);
			std::cout << BOLD BRIGHT_PURPLE << "[PMINFO] " << RESET << "received : " << BOLD BRIGHT_WHITE << "[" << complete_data.substr(0, complete_data.size() - 2) << "\\r\\n]" << RESET << " from client(" << (*pollfds)[index].fd << ")" << std::endl;
			partial_data[(*pollfds)[index].fd] = partial_data[(*pollfds)[index].fd].substr(pos + 1);
			Parser irc_parser(*requests, complete_data.c_str(), (*pollfds)[index].fd);
		}
	}
}

/**
 * @brief Creates a new context for a request handler.
 * 
 * @param req The request object.
 * @return Context* The newly created context.
 */
Context* PollManager::create_context_for_handler(Request* req) {
	Context* context = new Context;
	context->request = req;
	context->users = this->users;
	context->channels = channels;
	context->server_info = server;
	context->pollfds = pollfds;
	context->partial_data = &partial_data;
	context->requests = requests;
	return context;
}

/**
 * @brief Manages the requests in the PollManager.
 * 
 * This function handles the requests in the PollManager by iterating through the requests queue
 * and processing each request using a RequestHandler. It limits the number of requests to handle
 * to NREQUESTSTOHANDLE.
 */
void PollManager::manage_requests() {
	int index = 0;
	while (!this->requests->empty() && index < NREQUESTSTOHANDLE) {
		Request* request = this->requests->front();
		RequestHandler handler;
		try {
			Context* context = create_context_for_handler(request);
			handler.handle_request(context);
			delete context;
		} catch (std::exception& e) {
			std::cout << BOLD BRIGHT_YELLOW << "[PMWARN]" << RESET << "Something bad happened while handeling request" << std::endl;
		}
		this->requests->pop();
		delete request;
		index++;
	}
}

/**
 * Sends a response to the client at the specified index.
 * 
 * @param index The index of the client in the pollfds array.
 */
void PollManager::send_data(size_t index) {
	int user_fd = (*pollfds)[index].fd;
	User* user = users->find(user_fd)->second;
	if (!user->has_response())
		return ;
	Response* res = user->get_next_response();
	std::cout << BOLD BRIGHT_PURPLE << "[PMINFO]" << RESET << " sending a response to client(" << user_fd << ")" << std::endl;
	send(user_fd, res->get_response(), res->get_size(), 0);
	delete res;
}

void PollManager::create_bot() {
	User* bot = new User(server->fd);
	bot->set_nickname("bot");
	bot->set_username("bot");
	bot->set_authenticated(true);
	bot->set_registered(true);
	this->users->insert(std::make_pair(server->fd, bot));
}

/**
 * @brief Constructs a PollManager object.
 * 
 * This constructor initializes a PollManager object with the provided parameters.
 * It sets the server, users, pollfds, requests, and channels member variables.
 * It also adds a new pollfd to the pollfds vector and initializes it with the server file descriptor.
 * The constructor then enters a loop where it continuously polls the file descriptors for events.
 * If the poll function fails, it throws a PMFailureException.
 * If a new connection is detected on the server file descriptor, it calls the handle_new_connection function.
 * For each client file descriptor in the pollfds vector, it checks for POLLOUT and POLLIN events and calls the corresponding functions.
 * Finally, it calls the manage_requests function to handle any pending requests.
 * 
 * @param server A pointer to the server_info struct.
 * @param pollfds A reference to the vector of pollfd structs.
 * @param users A reference to the unordered_map of user file descriptors and User objects.
 * @param requests A reference to the queue of Request pointers.
 * @param channels A reference to the map of channel names and Channel objects.
 */
PollManager::PollManager(server_info* server, std::vector<struct pollfd>& pollfds, std::unordered_map<int, User*>& users, std::queue<Request*>& requests, std::map<std::string, Channel*>&channels) {
	this->server = server;
	this->users = &users;
	this->pollfds = &pollfds;
	this->requests = &requests;
	this->channels = &channels;
	this->pollfds->push_back(pollfd());
	pollfds[0].fd = server->fd;
	pollfds[0].events = POLLIN | POLLOUT;
	create_bot();
	while (keepRunning) {
		if (poll(&pollfds[0], this->pollfds->size(), TIMEOUT) == -1) {
			if (errno == EINTR) {
				std::cout << BOLD BRIGHT_YELLOW << "[PMWARN] " << RESET << "poll has been interrupted by a signal" << RESET << std::endl;
				continue;
			}
			throw PMFailureException("[PMERROR] poll has been failed");
		}

		if (pollfds[0].revents & POLLIN)
			handle_new_connection();

		for (size_t i = 0; i < this->pollfds->size(); i++) {
			if (pollfds[i].revents & POLLOUT)
				send_data(i);
			if (i > 0 && pollfds[i].revents & POLLIN)
				handle_client_activity(i);
		}
		manage_requests();
	}
}

PollManager::~PollManager() {}

const char* PollManager::PMFailureException::what() const throw() {
	return msg;
}