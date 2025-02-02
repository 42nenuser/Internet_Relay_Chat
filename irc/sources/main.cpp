#include "Server.hpp"
#include "globals.h"

volatile sig_atomic_t keepRunning = true;

/**
 * @brief Signal handler function that sets the global variable keepRunning to false.
 * 
 * @param signum The signal number.
 */
void signal_handler(int signum) {
	(void)signum;
	keepRunning = false;
}

/**
 * @brief Checks if a given port number is valid.
 * 
 * @param port The port number to check.
 * @return true if the port number is valid, false otherwise.
 */
bool is_valid_port(int port) {
	return (port >= 6660 && port <= 6669);
}

/**
 * @brief The entry point of the program.
 *
 * @param ac The number of command-line arguments.
 * @param av An array of command-line arguments.
 * @return int The exit status of the program.
 */
int	main(int ac, char **av) {
	if (ac != 3 || !is_valid_port(atoi(av[1]))) {
		std::cerr << BOLD BRIGHT_BLUE << "[Usage] : " << BOLD BRIGHT_PURPLE << "./ircserv <port 6660-6669> <password>" << RESET << std::endl;
		return FAILURE;
	}
	signal(SIGINT, signal_handler);
	try {
		Server	app(atoi(av[1]), std::string(av[2]));
	} catch (std::exception &e) {
		std::cerr << BOLD BRIGHT_RED << e.what() << std::endl;
		return FAILURE;
	}
	return SUCCESS;
}