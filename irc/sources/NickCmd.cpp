#include "NickCmd.hpp"

/**
 * Checks if the given character is a valid first character for a nickname.
 * A valid first character can be an alphabetic character or an underscore.
 *
 * @param c The character to check.
 * @return True if the character is a valid first character, false otherwise.
 */
bool is_first_valid(char c) {
	return (isalpha(c) || c == '_');
}

/**
 * Checks if a character is a valid middle character for a nickname.
 * A valid middle character can be alphanumeric, underscore (_) or hyphen (-).
 *
 * @param c The character to be checked.
 * @return True if the character is a valid middle character, false otherwise.
 */
bool is_mid_valid(char c) {
	return (isalnum(c) || c == '_' || c == '-');
}

/**
 * Checks if the given nickname has any invalid characters.
 * 
 * @param c The context object.
 * @return True if the nickname has invalid characters, false otherwise.
 */
bool NickCmd::has_invalid_char(Context* c) {
	size_t counter = 1;
	Request* req = c->request;
	User* user = c->users->find(req->get_fd())->second;
	std::string nickname = req->get_options();
	if (!is_first_valid(nickname[0])) {
		generate_response(user, rpl::erroneous_nickname(*user, req->get_options()));
		return true;
	}
	while (nickname[counter] && counter < 9 && is_mid_valid(nickname[counter]))
		counter++;
	req->set_options(req->get_options().substr(0, counter));
	return false;
}

/**
 * Checks if the requested nickname is already in use by another user.
 * If the nickname is already in use, it generates a response indicating that the nickname is already in use.
 * 
 * @param c The context object containing the request and user information.
 * @return True if the nickname is already in use, false otherwise.
 */
bool NickCmd::is_duplicate(Context* c) {
	Request* req = c->request;
	User* user = c->users->find(req->get_fd())->second;
	std::unordered_map<int, User*>::iterator it = c->users->begin();
	for (; it != c->users->end(); it++) {
		if (req->get_options() == it->second->get_nickname()) {
			generate_response(user, rpl::nick_already_in_use(*user, req->get_options()));
			return true;
		}
	}
	return false;
}

/**
 * Checks if the given nickname is valid.
 * 
 * @param context The context object containing the nickname.
 * @return True if the nickname is valid, false otherwise.
 */
bool NickCmd::is_nickname_valid(Context* context) {
	if (!is_duplicate(context) && !has_invalid_char(context))
		return true;
	return false;
}

/**
 * Executes the Nick command.
 * This function is responsible for handling the Nick command, which allows a user to set or change their nickname.
 * If the user is not authenticated, it generates a response indicating that the user is unregistered.
 * If the user is authenticated but does not have a nickname set, it checks if the provided nickname is valid and sets it.
 * If the user is authenticated and already has a nickname set, it checks if the provided nickname is valid and updates it.
 * If the provided nickname is the same as the current nickname, no action is taken.
 * After setting or updating the nickname, it generates an appropriate response.
 *
 * @param context The context object containing the request and user information.
 */
void NickCmd::execute(Context* context) {
	Request* req = context->request;
	User* user = context->users->find(req->get_fd())->second;
	if (!user->is_authenticated()) {
		generate_response(user, rpl::unregistered());
		return ;
	} else {
		if (!user->is_nickname_set()) {
			if (is_nickname_valid(context)) {
				user->set_nickname(req->get_options());
				if (user->is_username_set()) {
					user->set_registered(true);
					generate_response(user, rpl::welcome(*user));
				}
			}
		} else {
			if (req->get_options() == user->get_nickname())
				return ;
			if (is_nickname_valid(context)) {
				generate_response(user, rpl::nickname_set(*user, req->get_options()));
				user->set_nickname(req->get_options());
			}
		}
	}
}

/**
 * Generates a response for the NickCmd command.
 * Adds the response to the user's list of responses.
 *
 * @param user The user object.
 * @param response The response string.
 */
void NickCmd::generate_response(User* user, std::string const response) {
	Response* res = new Response(response);
	user->add_response(res);
}
