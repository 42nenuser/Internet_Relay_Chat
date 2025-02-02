#include "Request.hpp"

Request::Request(int fd) {
	this->fd = fd;
}

void Request::set_cmd(std::string cmd) {
	this->cmd = cmd;
}

void Request::set_options(std::string options) {
	this->options = options;
}

int Request::get_fd() const {
	return this->fd;
}

std::string Request::get_cmd() const {
	return this->cmd;
}

std::string Request::get_options() const {
	return this->options;
}