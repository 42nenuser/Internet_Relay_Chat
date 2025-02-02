#include "IRCBot.hpp"
#include "KickCmd.hpp"

/**
 * @brief Constructor for the IRCBot class.
 * 
 * @param context Pointer to the Context object.
 * @param message The message to be sent by the bot.
 * @param channel_name The name of the channel where the bot will send the message.
 */
IRCBot::IRCBot(Context* context, std::string message, std::string channel_name) {
	Request* req = context->request;
	sender = context->users->find(req->get_fd())->second;
	channel = context->channels->find(channel_name)->second;
	data = message;
	bot = context->users->find(context->server_info->fd)->second;
	ch_name = channel_name;
	this->context = context;
}

/**
 * Checks if the IRC bot is valid.
 *
 * This function checks if the IRC bot is valid by determining if it contains any insults.
 * If an insult is found, the type of the bot is set to INSULT and the function returns false.
 * Otherwise, it returns true.
 *
 * @return true if the IRC bot is valid, false otherwise.
 */
bool IRCBot::is_valid() {
	if (is_insult()) {
		type = INSULT;
		return false;
	}
	return true;
}

/**
 * Checks if the received data contains any insults.
 * 
 * @return true if the data contains insults, false otherwise.
 */
bool IRCBot::is_insult() {
	std::string insults[5] = {"retard", "fuck", "mklkh", "7mar", "hmar"};
	for (int i = 0; i < 5; i++) {
		if (data.find(insults[i]) != std::string::npos)
			return true;
	}
	return false;
}

/**
 * Takes action based on the type of the IRCBot.
 * If the type is INSULT, it creates a new Request object with the appropriate command and options,
 * and adds it to the request queue.
 */
void IRCBot::take_action() {
	if (type == INSULT) {
		Request* req = new Request(context->server_info->fd);
		req->set_cmd("KICK");
		req->set_options(ch_name + " " + sender->get_nickname());
		context->requests->push(req);
	}
}