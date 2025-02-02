#include "reply.hpp"

std::string const rpl::welcome001(User& user) {
	std::string reply = SERVER_PREFIX " 001 ";
	reply += user.get_nickname();
	reply += " :Welcome to the Internet Relay Network ";
	reply += user.get_nickname();
	reply += "!";
	reply += user.get_username();
	reply += "@";
	reply += HOSTNAME;
	reply += "\r\n";
	return reply;
}

std::string const rpl::host002(User& user) {
	std::string reply = SERVER_PREFIX " 002 ";
	reply += user.get_nickname();
	reply += " :Your host is ";
	reply += SERVER_NAME;
	reply += ", running version ";
	reply += SERVER_VERSION;
	reply += "\r\n";
	return reply;
}

std::string const rpl::created003(User& user) {
	std::string reply = SERVER_PREFIX " 003 ";
	reply += user.get_nickname();
	reply += " :This server was created ";
	reply += SERVER_CREATION_DATE;
	reply += "\r\n";
	return reply;
}

std::string const rpl::info004(User& user) {
	std::string reply = SERVER_PREFIX " 004 ";
	reply += user.get_nickname();
	reply += " :";
	reply += SERVER_NAME;
	reply += " ";
	reply += SERVER_VERSION;
	reply += "\r\n";
	return reply;
}

std::string const rpl::welcome(User& user) {
	std::string reply = welcome001(user);
	reply += host002(user);
	reply += created003(user);
	reply += info004(user);
	return reply;
}

std::string const rpl::unregistered() {
	std::string reply = SERVER_PREFIX " ";
	reply += "451 ";
	reply += "* :You have not registered";
	reply += "\r\n";
	return reply;
}

std::string const rpl::nickname_set(User& user, std::string new_nickname) {
	std::string reply = ":";
	reply += user.get_nickname();
	reply += "!";
	reply += user.get_username();
	reply += "@";
	reply += HOSTNAME;
	reply += " NICK :";
	reply += new_nickname;
	reply += "\r\n";
	return reply;
}

std::string const rpl::nick_already_in_use(User& user, std::string new_nickname) {
	std::string reply = SERVER_PREFIX;
	reply += " 433 ";
	reply += user.get_nickname();
	reply += " ";
	reply += new_nickname;
	reply += " :Nickname is already in use.";
	reply += "\r\n";
	return reply;
}

std::string const rpl::erroneous_nickname(User& user, std::string new_nickname) {
	std::string reply = SERVER_PREFIX;
	reply += " 432 ";
	reply += user.get_nickname();
	reply += " ";
	reply += new_nickname;
	reply += " :Erroneous Nickname";
	reply += "\r\n";
	return reply;
}

std::string const rpl::reregister(User& user) {
	std::string reply = SERVER_PREFIX;
	reply += " 462 ";
	reply += user.get_nickname();
	reply += " :You may not reregister";
	reply += "\r\n";
	return reply;
}

std::string const rpl::not_enough_params(User& user) {
	std::string reply = SERVER_PREFIX;
	reply += " 461 ";
	reply += user.get_nickname();
	reply += " :Not enough parameters";
	reply += "\r\n";
	return reply;
}

std::string const rpl::incorrect_server_password() {
	std::string reply = SERVER_PREFIX;
	reply += " 464 * :Password incorrect";
	reply += "\r\n";
	return reply;
}

std::string const rpl::no_such_channel(User& user, std::string channel_name) 
{
	  std::string reply = SERVER_PREFIX " 403 ";
    reply += user.get_nickname();
    reply += " ";
    reply += channel_name;
    reply += " :No such channel";
    reply +="\r\n"; 
    return(reply);
}

std::string const rpl::join_channel(User& user, std::string channel_name)
{
    std::string reply = ":";
    reply += user.get_nickname() + "!";
    reply +=  user.get_username() + "@" + HOSTNAME;
    reply += " JOIN :" + channel_name +  "\r\n";
    return(reply);
}

std::string const rpl::reply_topic(User& user, std::string channel_name, std::string topic)
{
    std::string reply = SERVER_PREFIX " 332 ";
    reply += user.get_nickname() + " " + channel_name + " ";
    reply += topic;
    reply += "\r\n";
    return(reply);
}

std::string const rpl::reply_names(User& user, std::string channel_name, std::string state, std::string members)
{
    std::string reply = SERVER_PREFIX " 353 ";
    reply += user.get_nickname();
    reply += " = ";
    reply += channel_name;
    reply += " :";
    reply += state;
    reply += user.get_nickname();
    reply += " ";
    reply += members;
    reply += "\r\n";
    return reply;
}

std::string const rpl::reply_end_of_names(User& user, std::string channel_name) 
{
    std::string reply = SERVER_PREFIX " 366 ";
    reply += user.get_nickname();
    reply += " " + channel_name;
    reply += " :End of NAMES list";
    reply += "\r\n";
    return reply;
}

std::string const rpl::reply_channel_mode_is(User& user, std::string channel_name) 
{
    std::string reply = SERVER_PREFIX " 324 ";
    reply += user.get_nickname() + " " + channel_name + " +";
    reply += "\r\n";
    return(reply);
}

std::string const rpl::reply_kick_user(User& user, std::string user_kicked, std::string channel_name)
{
    std::string reply = ":" + user.get_nickname() + "!" + user.get_username() + "@" + HOSTNAME;
    reply += " KICK "  + channel_name + " " + user_kicked;
    reply += "\r\n";
    return(reply);
}

std::string const rpl::reply_are_not_on_channel(User& user, std::string user_kicked, std::string channel_name)
{
    std::string reply = SERVER_PREFIX " 441 ";
    reply += user.get_nickname() + " " + user_kicked + " " + channel_name;
    reply += " :They aren't on that channel";
    reply += "\r\n";
    return(reply);
}

std::string const rpl::reply_you_are_not_channel_operator(User &user, std::string channel_name)
{
    std::string reply = SERVER_PREFIX " 482 ";
    reply += user.get_nickname() + " " + channel_name;
    reply += " :You're not channel operator";
    reply += "\r\n";
    return(reply);
}

std::string const rpl::no_text_to_send(User& user) {
    std::string reply = user.get_nickname();
    reply += " :No text to send";
    reply += "\r\n";
    return reply;
}

std::string const rpl::privmsg_user(User& sender, User& receiver, std::string message) {
    std::string reply = ":";
    reply += sender.get_nickname();
    reply += "!";
    reply += sender.get_username();
    reply += "@";
    reply += HOSTNAME;
    reply += " PRIVMSG ";
    reply += receiver.get_nickname();
    reply += " :";
    reply += message;
    reply += "\r\n";
    return reply;
}

std::string const rpl::privmsg_channel(User& sender, std::string receiver, std::string message) {
    std::string reply = ":";
    reply += sender.get_nickname();
    reply += "!";
    reply += sender.get_username();
    reply += "@";
    reply += HOSTNAME;
    reply += " PRIVMSG ";
    reply += receiver;
    reply += " :";
    reply += message;
    reply += "\r\n";
    return reply;
}

std::string const rpl::no_such_nick(User& user, std::string nickname) 
{
    std::string reply = SERVER_PREFIX " 401 ";
    reply += user.get_nickname();
    reply += " ";
    reply += nickname;
    reply += " :No such nick/channel";
    reply += "\r\n";
    return reply;
}

std::string const rpl::cannot_send_to_channel(User& user, std::string channel_name) {
    std::string reply = SERVER_PREFIX " 404 ";
    reply += user.get_nickname();
    reply += " ";
    reply += channel_name;
    reply += " :Cannot send to channel";
    reply += "\r\n";
    return reply;
}

std::string const rpl::reply_set_topic(User &user,std::string channel_name ,std::string topic)
{
    std::string reply = ":" + user.get_nickname() + "!" + user.get_username();
    reply += "@";
    reply += HOSTNAME;
    reply += " TOPIC ";
    reply += channel_name;
    reply += " " + topic;
    reply += "\r\n";
    return (reply);
}

std::string const rpl::reply_exist_user_and_channel(User &user, std::string nickname, std::string channel_name)
{
    std::string reply = SERVER_PREFIX " 341 ";
    reply += user.get_nickname();
    reply += " ";
    reply += nickname;
    reply += " ";
    reply += channel_name;
    reply += "\r\n";
    return(reply);

}

std::string const rpl::reply_invite_user(User &user, std::string nickname, std::string channel_name)
{
    std::string reply = ":";
    reply += user.get_nickname() + "!";
    reply += user.get_username() + "@";
    reply += HOSTNAME;
    reply += " INVITE ";
    reply += nickname;
    reply += " " + channel_name;
    reply += "\r\n";
    return(reply);
}

std::string const rpl::you_are_not_on_channel(User &user, std::string channel_name)
{
    std::string reply = SERVER_PREFIX " 442 ";
    reply += user.get_nickname();
    reply += " ";
    reply += channel_name;
    reply += " :You're not on that channel";
    reply += "\r\n";
    return(reply);
}

std::string const rpl::notify_operator_privilege_change(User &user, std::string channel_name, std::string option_mode, std::string nickname)
{
    std::string reply = ":";
    reply += user.get_nickname();
    reply += "!";
    reply += user.get_username();
    reply += "@";
    reply += HOSTNAME;
    reply += " MODE ";
    reply += channel_name;
    reply += " " + option_mode;
    reply += " " + nickname;
    reply += "\r\n";
    return(reply);
}

std::string const rpl::reply_rules_channel(User &user, std::string channel_name, std::string option_mode)
{
    std::string reply = ":";
    reply += user.get_nickname();
    reply += "!";
    reply += user.get_username();
    reply += "@";
    reply += HOSTNAME;
    reply += " MODE ";
    reply += channel_name;
    reply += " " + option_mode;
    reply += "\r\n";
    return(reply);
    
}

std::string const rpl::reply_you_can_not_join_channel(User &user, std::string channel_name)
{
    std::string reply = SERVER_PREFIX " 442 ";
    reply += user.get_nickname();
    reply += " ";
    reply += channel_name;
    reply += " :Cannot join channel (+i)";
    reply += "\r\n";
    return(reply);
}


std::string const rpl::reply_invite_only(User &user, std::string channel_name)
{
    std::string reply = SERVER_PREFIX " 473 ";
    reply += user.get_nickname();
    reply += " ";
    reply += channel_name;
    reply += " :Cannot join channel (+i)";
    reply += "\r\n";
    return(reply);
}

std::string const rpl::reply_set_authentication_channel(User &user,std::string channel_name,std::string option_mode ,std::string authentication_info)
{
    std::string reply = ":";
    reply += user.get_nickname();
    reply += "!";
    reply += user.get_username();
    reply += "@";
    reply += HOSTNAME;
    reply += " MODE ";
    reply += channel_name;
    reply += " ";
    reply += option_mode;
    reply += " ";
    reply += authentication_info;
    reply += "\r\n";
    return(reply);
}

std::string const rpl::channel_key_already_set(User &user, std::string channel_name)
{
    std::string reply = SERVER_PREFIX " 467 ";
    reply += user.get_nickname();
    reply += " ";
    reply += channel_name;
    reply += " :Channel key already set";
    reply += "\r\n";
    return(reply);
}


std::string const rpl::reply_password_incorrect(User &user, std::string channel_name)
{
    std::string reply = SERVER_PREFIX " 475 ";
    reply += user.get_nickname();
    reply += " ";
    reply += channel_name;
    reply += " :Cannot join channel (+k)";
    reply += "\r\n";
    return(reply);
}

std::string const rpl::reply_users_limit(User &user, std::string channel_name)
{
    std::string reply = SERVER_PREFIX " 471 ";
    reply += user.get_nickname();
    reply += " ";
    reply += channel_name;
    reply += " :Cannot join channel (+l)";
    reply += "\r\n";
    return(reply);
}

std::string const rpl::unknown_mode(User& user,std::string authentication_info)
{
    std::string reply = SERVER_PREFIX " 472 ";
    reply += user.get_nickname();
    reply += " ";
    reply += authentication_info;
    reply += " :is unknown mode char to me";
    reply += "\r\n";
    return(reply);
    
}

std::string const rpl::no_topic_is_set(User &user,std::string channel_name)
{
     std::string reply = SERVER_PREFIX " 331 ";
    reply += user.get_nickname();
    reply += " ";
    reply += channel_name;
    reply += " :No topic is set.";
    reply += "\r\n";
    return(reply);
}

std::string const rpl::display_user_topic(User &user,std::string channel_name, std::string topic)
{
    std::string reply = SERVER_PREFIX " 332 ";
    reply += user.get_nickname();
    reply += " ";
    reply += channel_name;
    reply += " ";
    reply += topic;
    reply += "\r\n";
    return(reply);
}

std::string const rpl::display_topic_setter(User &setter_user,User &user,std::string channel_name, std::string time)
{
    std::string reply = SERVER_PREFIX " 333 ";
    reply += user.get_nickname();
    reply += " ";
    reply += channel_name;
    reply += " ";
    reply += setter_user.get_nickname();
    reply += "!";
    reply += setter_user.get_username();
    reply += "@";
    reply += HOSTNAME;
    reply += " ";
    reply += time;
    reply += "\r\n";
    return(reply);
}

std::string const rpl::quit(User& user, std::string reason) {
	std::string reply = ":";
	reply += user.get_nickname();
	reply += "!";
	reply += user.get_username();
	reply += "@";
	reply += HOSTNAME;
	reply += " QUIT :";
	reply += reason;
	reply += "\r\n";
	return reply;
}

std::string const rpl::is_already_on_channel(User &user, std::string nickname,std::string channel_name)
{
    std::string reply = SERVER_PREFIX " 443 ";
    reply += user.get_nickname();
    reply += " ";
    reply += nickname;
    reply += " ";
    reply += channel_name;
    reply += " :is already on channel";
    reply += "\r\n";
    return(reply);
}