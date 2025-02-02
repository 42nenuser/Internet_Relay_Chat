#include "ICommand.hpp"
#include <ctime>
#include <sstream>
#include <cmath>
#include <iomanip>

std::string Context::to_lower(std::string name_channel)
{
    std::string channel_name = name_channel;
    for (size_t i = 0; i < channel_name.length(); ++i) 
        channel_name[i] = std::tolower(channel_name[i]);
    return(channel_name);    
}

std::map<std::string, Channel *> ::iterator Context::is_exist_channel(std::string &name_channel)
{
    name_channel = to_lower(name_channel);
    std::map<std::string, Channel *>::iterator it = channels->find(name_channel);
    if(it != channels->end())
        return(it);
    return(channels->end());     
}

bool Context::is_user_on_that_channel(std::string nickname, std::string channel_name)
{
    std::map<std::string, Channel *>::iterator it = is_exist_channel(channel_name);
    if(it != channels->end())
    {
        Channel *channel = it->second;
        if(channel->is_exist_user(nickname))
            return(true);
    }
    return(false);
}

bool Context::is_operator(User *user, std::string channel_name)
{
    std::map<std::string, Channel *> ::iterator it =is_exist_channel(channel_name);
    if(it != channels->end())
    {
        Channel *channel = it->second;
        std::set<std::string>operators = channel->get_operators();
        std::set<std::string>::iterator it_set = operators.find(user->get_nickname());
        if(it_set != operators.end())
            return(true);
    }
    return(false);
}

std::string Context::convert_double_to_string(double num)
{
	std::stringstream ss;
	ss << std::fixed << num;
	return(ss.str());
}