#ifndef IRCBOT_HPP
# define IRCBOT_HPP

# include <iostream>
# include "ICommand.hpp"

typedef enum {
	INSULT
} violation;

class IRCBot {
	private:
		User* bot;
		User* sender;
		Channel* channel;
		Context* context;
		std::string data;
		std::string ch_name;
		violation type;

		bool is_insult();
	public:
		IRCBot(Context*, std::string, std::string);
		void take_action();
		bool is_valid();
};

#endif