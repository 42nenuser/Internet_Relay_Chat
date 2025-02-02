#include "QuitCmd.hpp"

/**
 * @brief Deletes the user's file descriptor from the pollfds vector.
 * 
 * This function iterates over the pollfds vector and removes the file descriptor
 * associated with the user specified by req from the vector. If the file descriptor
 * is found, it is erased from the vector and the function returns. If the file descriptor
 * is not found, the function does nothing.
 */
void QuitCmd::delete_user_from_pollfds() {
	std::vector<struct pollfd>::iterator it = pollfds->begin();
	for (; it != pollfds->end(); it++) {
		if (req->get_fd() == it->fd) {
			pollfds->erase(it);
			return ;
		}
	}
}

/**
 * @brief Deletes the user from the users list.
 * 
 * This function removes the user from the users list based on their file descriptor.
 * 
 * @return void
 */
void QuitCmd::delete_user_from_users_list() {
	users->erase(users->find(req->get_fd()));
}

/**
 * Checks if a user is in a given channel.
 * 
 * @param channel The channel to check.
 * @return True if the user is in the channel, false otherwise.
 */
bool QuitCmd::user_in_channel(Channel* channel) {
	std::vector<User*> ch_users = channel->get_users();
	std::vector<User*>::iterator it = ch_users.begin();
	for (; it != ch_users.end(); it++) {
		if (user->get_nickname() == (*it)->get_nickname())
			return true;
	}
	return false;
}

/**
 * Checks if the user is present in any channel.
 * @return true if the user is present in any channel, false otherwise.
 */
bool QuitCmd::user_in_any_channel() {
	std::map<std::string, Channel*>::iterator it = channels->begin();
	for (; it != channels->end(); it++) {
		if (user_in_channel((*it).second))
			return true;
	}
	return false;
}

/**
 * Sends a response to all users except the current user.
 *
 * @param users The vector of User pointers representing all connected users.
 * @param res The response message to be sent.
 */
void QuitCmd::send_to_all_users(std::vector<User*>& users, std::string res) {
	std::vector<User*>::iterator it = users.begin();
	for (; it != users.end(); it++) {
		if ((*it)->get_nickname() != user->get_nickname())
			(*it)->add_response(new Response(res));
	}
}

/**
 * @brief Informs all users in the channels the user is currently in about the quit event.
 * 
 * This function sends a quit message to all users in the channels that the user is currently in.
 * It also performs additional actions such as kicking the user, revoking operator status,
 * and removing the user from the invite list in each channel.
 * 
 * @param reason The reason for quitting.
 */
void QuitCmd::inform_all_users(std::string reason) {
	std::map<std::string, Channel*>::iterator it = channels->begin();
	std::string res = rpl::quit(*user, reason);
	for (; it != channels->end(); it++) {
		if (user_in_channel(it->second)) {
			send_to_all_users(it->second->get_users(), res);
			it->second->kick_user(user->get_nickname());
			it->second->revoke_operator_status(user->get_nickname());
			it->second->remove_user_from_invite_list(user->get_nickname());
		}
	}
}

/**
 * @brief Deletes the partial data associated with a file descriptor.
 * 
 * This function removes the partial data associated with the given file descriptor from the partial_data map.
 * 
 * @param fd The file descriptor for which the partial data needs to be deleted.
 */
void QuitCmd::delete_partial_data(int fd) {
	std::map<int, std::string>::iterator it = partial_data->find(fd);
	if (it != partial_data->end()) {
		partial_data->erase(it);
	}
}

/**
 * @brief Executes the Quit command, handling the disconnection of a client.
 * 
 * This function performs the following steps:
 * 1. Retrieves the necessary data from the context object.
 * 2. Checks if the user is in any channel and informs all users if necessary.
 * 3. Deletes the user from the pollfds, users list, and partial data.
 * 4. Closes the user's file descriptor.
 * 5. Prints a message indicating the client's disconnection.
 * 
 * @param context The context object containing the necessary data for execution.
 */
void QuitCmd::execute(Context* context) {
	pollfds = context->pollfds;
	req = context->request;
	users = context->users;
	channels = context->channels;
	user = users->find(req->get_fd())->second;
	partial_data = context->partial_data;
	if (user_in_any_channel())
		inform_all_users(req->get_options());
	delete_user_from_pollfds();
	delete_user_from_users_list();
	delete_partial_data(req->get_fd());
	delete user;
	close(req->get_fd());
	std::cout << BOLD BRIGHT_GREEN << "[CHINFO] " << RESET << "client(" << BOLD BRIGHT_CYAN << req->get_fd() << RESET << ") disconnected." << std::endl;
}

void QuitCmd::generate_response(User* user, std::string const response) {
	(void)user, (void)response;
}