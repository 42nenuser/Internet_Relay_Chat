#pragma once

#include "ICommand.hpp"

class JoinCmd : public ICommand 
{
	private:
		std::string channel_name;
		std::string dynamic_user_data;
	public:
		void set_dynamic_user_data(std::string);
		std::string get_dynamic_user_data()const;
		void create_channel(Context **);
		void add_creator_to_channel(Context **);
		void add_new_channel(Context *context,std::map<std::string, Channel *>::iterator it, User *user);
		void inform_operators_that_another_user_join_to_channel(User *user, Context *context, std::string &name_channel);
		void execute(Context*);
		void generate_response(User*, std::string const);
		bool is_valid_channel_name(std::string);
		void set_channel_name(std::string);
		std::string get_channel_name() const;
		void parse_request(std::string request);
		std::string get_members_string(Context *context, Channel *channel, User *user);
		const char*  user_status_prefix(User *user, Context *context, std::string);
		void join_channel(Context *context, User *user, Channel *channel);
		void handle_invitation_status(Channel *channel, User *user, std::string channel_name, Context *context);
		void handle_password_verification(Channel *channel,User *user, std::string channel_name, Context *context);
		void handle_user_limit_check(Channel*channel, User *user, std::string channel_name, Context *context);
		void handle_user_join_request(User *user, std::string channel_name, Context *contex);
};
