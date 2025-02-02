#ifndef REPLY_HPP
# define REPLY_HPP

# include <iostream>
# include "User.hpp"
# include "server_info.hpp"

namespace rpl {
	std::string	const welcome(User& user);
	std::string const welcome001(User& user);
	std::string const host002(User& user);
	std::string const created003(User& user);
	std::string const info004(User& user);
	std::string const unregistered();
	std::string const nickname_set(User& user, std::string new_nickname);
	std::string const nick_already_in_use(User& user, std::string new_nickname);
	std::string const erroneous_nickname(User& user, std::string new_nickname);
	std::string const reregister(User& user);
	std::string const not_enough_params(User& user);
	std::string const incorrect_server_password();
	std::string const no_such_channel(User& user, std::string channel_name);
	std::string const join_channel(User& user, std::string channel_name);
	std::string const reply_topic(User& user, std::string channel_name, std::string topic);
	std::string const reply_names(User&, std::string, std::string, std::string);
	std::string const reply_end_of_names(User& user, std::string channel_name);
	std::string const reply_channel_mode_is(User& user, std::string channel_name);
	std::string const reply_kick_user(User&, std::string, std::string);
	std::string const reply_are_not_on_channel(User &, std::string, std::string);
	std::string const reply_you_are_not_channel_operator(User&, std::string );
	std::string const no_text_to_send(User& user);
	std::string const privmsg_user(User& sender, User& receiver, std::string message);
	std::string const privmsg_channel(User& sender, std::string receiver, std::string message);
	std::string const no_such_nick(User& user, std::string nickname);
	std::string const cannot_send_to_channel(User& user, std::string channel_name);
	std::string const reply_set_topic(User &user,std::string channel_name ,std::string topic);
	std::string const reply_exist_user_and_channel(User &user, std::string nickname, std::string channel_name);
	std::string const reply_invite_user(User &user, std::string nickname, std::string channel_name);
	std::string const you_are_not_on_channel(User &user, std::string channel_name);
	std::string const notify_operator_privilege_change(User &user, std::string channel_name, std::string option_mode, std::string nickname);
	std::string const reply_rules_channel(User &user, std::string channel_name, std::string option_mode);
	std::string const reply_you_can_not_join_channel(User &user, std::string channel_name);
	std::string const reply_set_authentication_channel(User &, std::string ,std::string ,std::string);
	std::string const channel_key_already_set(User &, std::string);
	std::string const reply_password_incorrect(User &user, std::string channel_name);
	std::string const unknown_mode(User&,std::string);
	std::string const reply_users_limit(User &user, std::string channel_name);
	std::string const reply_invite_only(User &user, std::string channel_name);
	std::string const no_topic_is_set(User&user,std::string channel_name);
	std::string const display_user_topic(User &user,std::string channel_name, std::string topic);
	std::string const display_topic_setter(User &setter_user,User &user,std::string channel_name, std::string time);
	std::string const quit(User& user, std::string reason);
	std::string const is_already_on_channel(User &user, std::string nickname,std::string channel_name);	
}

#endif