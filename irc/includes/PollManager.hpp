#ifndef POLLMANAGER_HPP
# define POLLMANAGER_HPP

# include "Server.hpp"
# include "globals.h"

struct server_info;

class PollManager {
	private:
		server_info* server;
		std::unordered_map<int, User*>* users;
		std::vector<struct pollfd>* pollfds;
		std::queue<Request*>* requests;
		std::map<std::string, Channel*> *channels;
		std::map<int, std::string> partial_data;

		void handle_new_connection();
		void handle_client_activity(size_t);
		void remove_disconnected_client(size_t);
		void manage_requests();
		void send_data(size_t);
		Context* create_context_for_handler(Request*);
		void create_bot();
	public:
		PollManager(server_info*, std::vector<struct pollfd>&, std::unordered_map<int, User*>&, std::queue<Request*>&, std::map<std::string, Channel*>&);
		~PollManager();

		class PMFailureException : public std::exception {
			private:
				const char* msg;
			public:
				PMFailureException(const char* message) : msg(message) {};
				const char* what() const throw();
		};
};

#endif