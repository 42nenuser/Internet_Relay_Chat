#ifndef ICOMMAND_HPP
# define ICOMMAND_HPP

# include <unordered_map>
# include "User.hpp"
# include "Request.hpp"
# include "Channel.hpp"
# include "Color.h"
# include "Response.hpp"
# include <map>
# include "server_info.hpp"
# include "reply.hpp"
# include <poll.h>
# include <unistd.h>

class Context {
	public:
		Request* request;
		std::unordered_map<int, User*> *users;
		std::map<std::string, Channel *> *channels;
		server_info* server_info;
    	std::vector<struct pollfd> *pollfds;
		std::map<int, std::string> *partial_data;
		std::queue<Request*>* requests;
		std::string to_lower(std::string name_channel);
		std::map<std::string, Channel *> ::iterator is_exist_channel(std::string &name_channel);
		bool is_user_on_that_channel(std::string nickname, std::string channel_name);
		bool is_operator(User *user, std::string channel_name);
		std::string convert_double_to_string(double num);
};

class ICommand {
	public:
		virtual ~ICommand() {};
		virtual void execute(Context*) = 0;
		virtual void generate_response(User*, std::string const) = 0;
};

#endif
