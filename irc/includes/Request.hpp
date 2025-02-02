#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>

class Request {
	private:
		int fd;
		std::string cmd;
		std::string options;
	public:
		Request(int fd);
		void set_cmd(std::string);
		void set_options(std::string);
		int get_fd() const;
		std::string get_cmd() const;
		std::string get_options() const;
};

#endif