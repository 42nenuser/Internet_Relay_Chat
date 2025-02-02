#include "Parser.hpp"

/**
 * @brief Parses a string to extract a command and its options.
 * 
 * This function takes a string and extracts the command and its options.
 * The command is set in the provided Request object, while the options are set as well if present.
 * 
 * @param str The string to parse.
 * @param req Pointer to the Request object to store the parsed command and options.
 */
void Parser::get_command(std::string str, Request* req) {
	size_t del_pos = str.find(" ");
	req->set_cmd(str.substr(0, del_pos));
	if (del_pos == std::string::npos)
		return;
	req->set_options(str.substr(del_pos + 1));
}

/**
 * @brief Parses a string and creates a new Request object.
 * 
 * @param str The string to parse.
 * @param id The ID of the request.
 * @return A pointer to the created Request object.
 */
Request* Parser::get_request(std::string str, int id) {
	Request* req = new Request(id);

	get_command(str, req);
	return req;
}

/**
 * Splits the given buffer into individual requests using the specified delimiter.
 * Each request is added to the provided queue of requests.
 *
 * @param requests The queue of requests to add the split requests to.
 * @param buffer The buffer containing the requests to be split.
 * @param del The delimiter used to split the requests.
 * @param id The identifier for the requests.
 */
void Parser::split_requests(std::queue<Request*>& requests, std::string buffer, const char* del, int id) {
	std::string request;
	size_t del_pos = buffer.find(del);
	while (del_pos != std::string::npos) {
		request = buffer.substr(0, del_pos);
		buffer.erase(0, del_pos + strlen(del));
		requests.push(get_request(request, id));
		del_pos = buffer.find(del);
	}
}

/**
 * @brief Checks if a request is valid.
 * 
 * @param buffer The request buffer.
 * @return true if the request is valid, false otherwise.
 */
bool Parser::is_valid_request(std::string buffer) {
	size_t len = buffer.size();
	return (buffer[len - 1] == '\n' && buffer[len - 2] == '\r');
}

/**
 * @brief Refactors the request buffer by removing the last character and appending "\r\n\0" at the end.
 * 
 * @param buffer The request buffer to be refactored.
 */
void Parser::refactor_request(std::string& buffer) {
	buffer.erase(buffer.size() - 1);
	buffer.insert(buffer.size(), "\r\n\0");
}

/**
 * @brief Constructs a Parser object.
 * 
 * This constructor takes a queue of Request pointers, a string buffer, and an ID as parameters.
 * It checks if the request in the buffer is valid, and if not, it refactors the request.
 * Then, it splits the requests in the buffer using the specified delimiter "\r\n" and adds them to the queue.
 * 
 * @param requests The queue of Request pointers to store the parsed requests.
 * @param buffer The string buffer containing the requests to be parsed.
 * @param id The ID of the Parser object.
 */
Parser::Parser(std::queue<Request*>& requests, std::string buffer, int id) {
	if (!is_valid_request(buffer))
		refactor_request(buffer);
	split_requests(requests, buffer, "\r\n", id);
}
