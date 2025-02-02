#ifndef PARSER_HPP
# define PARSER_HPP

# include <iostream>
# include <queue>
# include "Request.hpp"

class Parser {
	private:
		void split_requests(std::queue<Request*>&, std::string, const char*, int);
		Request* get_request(std::string, int);
		void get_command(std::string, Request*);
		bool is_valid_request(std::string buffer);
		void refactor_request(std::string& buffer);
	public:
		Parser(std::queue<Request*>&, std::string, int);
};

#endif