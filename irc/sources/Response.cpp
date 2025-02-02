#include "Response.hpp"

Response::Response(std::string res) {
	this->response = res;
}

Response::~Response() {}

const char* Response::get_response() const {
	return this->response.c_str();
}

size_t Response::get_size() const {
	return this->response.size();
}