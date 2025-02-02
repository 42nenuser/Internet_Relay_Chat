#include "ConnectionManager.hpp"

/**
 * @brief Constructs a ConnectionManager object.
 * 
 * This constructor initializes the server by creating a socket, 
 * initializing the address structure, binding the socket, and 
 * listening for incoming connections.
 * 
 * @param info A pointer to the server_info struct containing server configuration information.
 * @throws CMFailureException if any of the initialization steps fail.
 */
ConnectionManager::ConnectionManager(server_info* info) {
	std::cout << BOLD BRIGHT_BLUE << "[CMINFO]" << RESET << " initializing server..." << RESET << std::endl;
	this->info = info;
	
	try {
		create_socket();
		init_addr_struct();
		bind_socket();
		listen_with_socket();
	} catch (std::exception& e) {
		throw CMFailureException(e.what());
	}
}

ConnectionManager::~ConnectionManager() {}

/**
 * @brief Creates a socket for the server.
 *
 * This function creates a socket using the AF_INET domain, SOCK_STREAM type, and 0 protocol.
 * If the socket creation fails, it throws a std::runtime_error.
 * The socket is set to non-blocking mode using fcntl.
 * If setting the socket to non-blocking mode fails, it throws a std::runtime_error.
 */
void	ConnectionManager::create_socket() {
	info->fd = socket(AF_INET, SOCK_STREAM, 0);
	if (info->fd == -1)
		throw std::runtime_error("[CMERROR] couldn't create server socket");
	std::cout << BOLD BRIGHT_BLUE << "[CMINFO]" << RESET << " server socket created" << RESET << std::endl;

	if (fcntl(info->fd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("[CMERROR] couldn't set server socket to non-blocking mode");
	std::cout << BOLD BRIGHT_BLUE << "[CMINFO]" << RESET << " server socket set to non-blocking mode" << RESET << std::endl;
}

/**
 * Binds the server socket to the specified address and port.
 * Throws a std::runtime_error if the binding fails.
 */
void	ConnectionManager::bind_socket() {
	if (bind(info->fd, reinterpret_cast<struct sockaddr*>(&info->addr), sizeof info->addr) == -1)
		throw std::runtime_error("[CMERROR] couldn't bind server socket (probably the port is already in use)");
	std::cout << BOLD BRIGHT_BLUE << "[CMINFO]" << RESET << " server socket bound with 0.0.0.0:" << info->port << RESET << std::endl;
}

/**
 * @brief Listens for incoming connections on the server socket.
 * 
 * This function sets the server socket to listen for incoming connections. 
 * If the socket fails to set for listening, a std::runtime_error is thrown.
 */
void	ConnectionManager::listen_with_socket() {
	std::cout << BOLD BRIGHT_BLUE << "[CMINFO]" << RESET << " server socket is listening and ready to accept new connections..." << RESET << std::endl;
	if (listen(info->fd, SOMAXCONN) == -1)
		throw std::runtime_error("[CMERROR] couldn't set socket for listening");
}

/**
 * @brief Initializes the address structure for the connection.
 * 
 * This function sets the address structure to zero using memset,
 * sets the address family to AF_INET, sets the IP address to INADDR_ANY,
 * and sets the port number to the value stored in the 'port' member variable.
 */
void	ConnectionManager::init_addr_struct() {
	memset(&info->addr, 0, sizeof info->addr);
	info->addr.sin_family = AF_INET;
	info->addr.sin_addr.s_addr = htonl(INADDR_ANY);
	info->addr.sin_port = htons(info->port);
}

/**
 * @brief Returns the error message associated with the CMFailureException.
 * 
 * @return const char* The error message.
 */
const char* ConnectionManager::CMFailureException::what() const throw() {
	return msg;
}
