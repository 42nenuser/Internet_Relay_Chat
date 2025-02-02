#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>

class Response {
	private:
		std::string response;
	public:
		Response(std::string res);
		~Response();
		const char* get_response() const;
		size_t get_size() const;
};

#endif