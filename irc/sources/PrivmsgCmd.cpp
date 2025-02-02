#include "PrivmsgCmd.hpp"
#include "IRCBot.hpp"

/**
 * @brief Sets the receiver type based on the provided receiver string.
 * 
 * If the receiver string starts with '#', the receiver type is set to CHANNEL.
 * Otherwise, the receiver type is set to USER.
 * 
 * @param receiver The receiver string.
 */
void PrivmsgCmd::set_receiver_type(std::string receiver) {
	if (receiver[0] == '#') {
		type = CHANNEL;
		return ;
	}
	type = USER;
}

/**
 * Parses the request options to extract the receiver and message.
 * 
 * @param req The request object containing the options.
 * @return True if the parsing is successful, false otherwise.
 */
bool PrivmsgCmd::parse_request(Request* req) {
	std::string str = req->get_options();
	size_t del_pos = str.find(' ');
	if (del_pos == std::string::npos)
		return false;
	receiver = str.substr(0, del_pos);
	message = str.substr(del_pos + 1);
	if (message[0] == ':')
		message = message.substr(1);
	set_receiver_type(receiver);
	return true;
}

/**
 * @brief Finds the receiver of a private message in the given context.
 * 
 * @param context The context containing the users.
 * @return true if the receiver is found, false otherwise.
 */
bool PrivmsgCmd::find_receiver(Context* context) {
	std::unordered_map<int, User*>::iterator it = context->users->begin();
	for (; it != context->users->end(); it++) {
		if (receiver == it->second->get_nickname()) {
			receiv = it->second;
			return true;
		}
	}
	return false;
}

/**
 * @brief Finds the channel associated with the receiver in the given context.
 * 
 * @param context The context containing the channels.
 * @return true if the channel is found, false otherwise.
 */
bool PrivmsgCmd::find_channel(Context* context) {
	std::map<std::string, Channel*>::iterator it = context->channels->begin();
	for (; it != context->channels->end(); it++) {
		if (receiver == it->first) {
			channel = it->second;
			return true;
		}
	}
	return false;
}

/**
 * Checks if the sender is present in the given list of users.
 * 
 * @param sender The user to check for presence.
 * @param users The list of users to search in.
 * @return True if the sender is found in the list, false otherwise.
 */
bool PrivmsgCmd::is_sender_in_channel(User* sender, std::vector<User*> users) {
	std::vector<User*>::iterator it = users.begin();
	for (; it != users.end(); it++) {
		if (sender->get_nickname() == (*it)->get_nickname())
			return true;
	}
	return false;
}

/**
 * Sends a message to all users in a channel, excluding the sender.
 *
 * @param sender The user sending the message.
 * @param message The message to be sent.
 * @param channel The channel to send the message to.
 */
void PrivmsgCmd::send_message_to_channel(User* sender, std::string message, Channel* channel) {
	std::vector<User*> users = channel->get_users();
	std::vector<User*>::iterator it = users.begin();
	for (; it != users.end(); it++) {
		if (sender->get_nickname() != (*it)->get_nickname())
			generate_response(*it, message);
	}
}

/**
 * Executes the PRIVMSG command.
 * This command is used to send a message to a user or a channel.
 * If the sender is not authenticated, it generates a response indicating that the sender is unregistered.
 * If the request cannot be parsed, it generates a response indicating that there is no text to send.
 * If the message is intended for a user, it checks if the receiver exists and generates a response accordingly.
 * If the message is intended for a channel, it checks if the channel exists and if the sender is a member of the channel,
 * then sends the message to the channel.
 * If the channel or receiver does not exist, it generates a response indicating that there is no such channel or receiver.
 *
 * @param context The context object containing the request and user information.
 */
void PrivmsgCmd::execute(Context* context) {
	Request* req = context->request;
	User* sender = context->users->find(req->get_fd())->second;
	if (!sender->is_authenticated()) {
		generate_response(sender, rpl::unregistered());
		return ;
	}
	if (parse_request(req) == false) {
		generate_response(sender, rpl::no_text_to_send(*sender));
		return ;
	}
	if (type == USER) {
		if (find_receiver(context))
			generate_response(receiv, rpl::privmsg_user(*sender, *receiv, message));
		else
			generate_response(sender, rpl::no_such_nick(*sender, receiver));
		return ;
	}
	if (find_channel(context)) {
		if (is_sender_in_channel(sender, channel->get_users())) {
			IRCBot bot(context, message, receiver);
			if (bot.is_valid()) {
				send_message_to_channel(sender, rpl::privmsg_channel(*sender, receiver, message), channel);
				return ;
			}
			bot.take_action();
		}
		else
			generate_response(sender, rpl::cannot_send_to_channel(*sender, receiver));
		return ;
	}
	generate_response(sender, rpl::no_such_channel(*sender, receiver));
}

/**
 * Generates a response for the Privmsg command.
 * Adds the response to the user's response queue.
 *
 * @param user The user who sent the command.
 * @param response The response message to be added.
 */
void PrivmsgCmd::generate_response(User* user, std::string const response) {
	user->add_response(new Response(response));
}