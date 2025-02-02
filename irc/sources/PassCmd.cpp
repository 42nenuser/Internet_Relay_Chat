#include "PassCmd.hpp"

/**
 * Executes the PASS command.
 * This function checks if the user is already authenticated or not.
 * If the user is already authenticated, it prints an unauthorized command message.
 * If the password provided by the user matches the server password, it sets the user as authenticated.
 * If the password provided by the user does not match the server password, it generates a response with an incorrect server password message.
 *
 * @param context The context object containing the request and other relevant information.
 */
void PassCmd::execute(Context* context) 
{
	Request* req = context->request;
	User* user = context->users->find(req->get_fd())->second;
	std::string server_pass = context->server_info->password;
	if (user->is_authenticated())
		std::cout << BOLD BRIGHT_YELLOW << "[CHWARN] " << RESET << "Unauthorized command (already authenticated)" << std::endl;
	else if (server_pass == req->get_options()) {
		std::cout << BOLD BRIGHT_GREEN << "[CHINFO] " << RESET << "Password match (authenticated)" << std::endl;
		user->set_authenticated(true);
	}
	else {
		std::cout << BOLD BRIGHT_YELLOW << "[CHWARN]" << RESET << " Incorrect password." << std::endl;
		generate_response(user, rpl::incorrect_server_password());
	}
}

/**
 * Generates a response for the PassCmd.
 * Adds the response to the user's response queue.
 *
 * @param user The user object.
 * @param response The response string.
 */
void PassCmd::generate_response(User* user, std::string const response) {
	user->add_response(new Response(response));
}
