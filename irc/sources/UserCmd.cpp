#include "UserCmd.hpp"

/**
 * Splits a string into a vector of substrings based on a delimiter.
 * 
 * @param del The delimiter character used to split the string.
 * @return A vector of substrings obtained by splitting the string.
 */
std::vector<std::string> UserCmd::my_split(char del) {
	std::vector<std::string> vect;
	std::string options = req->get_options();
	size_t del_pos = 0;
	size_t start_pos = 0;
	do {
		del_pos = options.find(del, start_pos);
		std::string str = options.substr(start_pos, del_pos - start_pos);
		vect.push_back(str);
		start_pos = del_pos + 1;
	}
	while (del_pos != std::string::npos);
	return vect;
}

/**
 * @brief Parses the user parameters and sets the corresponding values in the User object.
 * 
 * This function splits the input string into individual parameters and checks if the number of parameters is correct.
 * If the number of parameters is not equal to 4, it sets the response type to INSUF_PARAMS and returns false.
 * Otherwise, it sets the username, hostname, and realname in the User object based on the parsed parameters.
 * If the nickname is already set in the User object, it sets the registered flag to true and generates a welcome response.
 * 
 * @return true if the user parameters are successfully parsed and set, false otherwise.
 */
bool UserCmd::get_user_params() {
	std::vector<std::string> params = my_split(' ');
	if (params.size() != 4) {
		res_type = INSUF_PARAMS;
		return false;
	}
	user->set_username(params[0]);
	user->set_hostname(params[2]);
	user->set_realname(params[3].substr(1));
	if (user->is_nickname_set()) {
		user->set_registered(true);
		generate_response(user, rpl::welcome(*user));
	}
	return true;
}

/**
 * Executes the UserCmd.
 * This function is responsible for executing the UserCmd based on the provided context.
 * It checks if the user is authenticated and if the necessary parameters are set.
 * If the user is not authenticated, it generates an unregistered response.
 * If the user is already registered or the required parameters are not provided, it generates an appropriate response.
 * @param context The context object containing the request and user information.
 */
void UserCmd::execute(Context* context) 
{
	this->req = context->request;
	this->user = context->users->find(req->get_fd())->second;
	if (!user->is_authenticated())
		generate_response(user, rpl::unregistered());
	else if (user->is_username_set() || !get_user_params()) {
		if (res_type != INSUF_PARAMS) {
			generate_response(user, rpl::reregister(*user));
			return ;
		}
		generate_response(user, rpl::not_enough_params(*user));
	}
}

/**
 * Generates a response for the user and adds it to the user's response list.
 * 
 * @param user The user for whom the response is generated.
 * @param response The response message to be added.
 */
void UserCmd::generate_response(User* user, std::string const response) {
	Response* res = new Response(response);
	user->add_response(res);
}