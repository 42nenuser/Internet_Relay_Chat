#include"KickCmd.hpp"

void KickCmd::set_channelname(std::string name_ch)
{
    this->channel_name = name_ch;
}

void KickCmd::set_username(std::string nickname)
{
    this->user_name = nickname;
}

std::string KickCmd::get_channel_name()
{
    return channel_name;
}

std::string KickCmd::get_username()
{
    return user_name;
}

/**
 * @brief Parses the channel name and user name from the request string.
 * 
 * This function extracts the channel name and user name from the given request string.
 * It iterates through the request string, separating the channel name and user name
 * based on the space character. The extracted channel name and user name are then
 * stored in the respective member variables of the KickCmd class.
 * 
 * @param request The request string containing the channel name and user name.
 */
void KickCmd::parse_channel_and_user(std::string &request)
{
    std::string channel_name;
    std::string user_name;
        
    size_t i = 0;
    for(; request[i] != ' ' ; i++)
        channel_name += request[i]; 
    i++;
    for(; i < request.size(); i++)
        user_name += request[i];
    if (user_name[user_name.size() - 1] == ':')
        user_name = user_name.substr(0, user_name.size() - 2);
    set_channelname(channel_name);
    set_username(user_name);       
}

/**
 * @brief Iterator type for iterating over elements in a std::map<std::string, Channel *>
 * 
 * This iterator is used to traverse the elements in a std::map<std::string, Channel *> container.
 * It provides access to both the key and value of each element in the map.
 * 
 * @tparam Key The key type of the map
 * @tparam T The value type of the map
 */
std::map<std::string, Channel *> ::iterator KickCmd::is_exist_channel(Context *context, std::string &name_channel)
{
    name_channel = context->to_lower(name_channel);
    std::map<std::string , Channel *> *channel = context->channels;
    std::map<std::string, Channel *>::iterator it = channel->find(name_channel);
    if(it != channel->end())
        return(it);
    return(channel->end());     
}

/**
 * Checks if a user is an operator in a specific channel.
 * 
 * @param user          The user to check.
 * @param context       The context containing the channels.
 * @param channel_name  The name of the channel to check in.
 * @return              True if the user is an operator, false otherwise.
 */
bool KickCmd::is_operator(User *user, Context *context, std::string channel_name)
{
    std::map<std::string, Channel *> ::iterator it =is_exist_channel(context, channel_name);
    if(it != context->channels->end())
    {
        Channel *channel = it->second;
        std::set<std::string>operators = channel->get_operators();
        std::set<std::string>::iterator it_set = operators.find(user->get_nickname());
        if(it_set != operators.end())
            return(true);
    }
    return(false);
}

/**
 * Handles the response for kicking a user from a channel.
 * 
 * @param user The user who initiated the kick command.
 * @param user_kicked The username of the user being kicked.
 * @param channel_name The name of the channel.
 * @param channel A pointer to the Channel object.
 */
void KickCmd::handle_response_for_kick_user(User* user, std::string user_kicked, std::string channel_name, Channel *channel)
{
    std::string response;
    std::vector<User *> users = channel->get_users();
    for(size_t i = 0; i < users.size(); i++)
        generate_response(users[i],rpl::reply_kick_user(*user, user_kicked, channel_name));
}

/**
 * Executes the Kick command.
 * This function kicks a user from a channel if the user is an operator of the channel.
 * If the user is not authenticated or registered, it generates an appropriate response.
 * If the channel or user does not exist, it generates an appropriate response.
 * If the user is not an operator of the channel, it generates an appropriate response.
 * After kicking the user, it revokes the operator status of the kicked user.
 *
 * @param context The context object containing the request and other necessary data.
 */
void KickCmd::execute(Context* context) 
{
    std::string channel_name;
    std::string user_kicked;
    std::string parse = context->request->get_options();
    User *user = context->users->find(context->request->get_fd())->second;
    if (!user->is_authenticated() || !user->is_registered()) {
        generate_response(user, rpl::unregistered());
        return ;
    }
    parse_channel_and_user(parse);
    channel_name = get_channel_name();
    user_kicked = get_username();
    if (user_kicked == context->users->find(context->server_info->fd)->second->get_nickname())
        return ;
    std::map<std::string, Channel *>::iterator it = is_exist_channel(context,channel_name);
    if(it != context->channels->end())
    {
        if(is_operator(user,context ,channel_name))
        {
            Channel *channel = it->second;
            if(channel->is_exist_user(user_kicked))
            {
                handle_response_for_kick_user(user, user_kicked, channel_name, channel);
                channel->kick_user(user_kicked);
                channel->revoke_operator_status(user_kicked);
            }
            else
                generate_response(user, rpl::reply_are_not_on_channel(*user, user_kicked, channel_name));
        }
        else
            generate_response(user, rpl::reply_you_are_not_channel_operator(*user, channel_name));
    }
    else
        generate_response(user, rpl::no_such_channel(*user, channel_name));
}

/**
 * Generates a response for the Kick command.
 * 
 * @param user The user who initiated the Kick command.
 * @param response The response message to be added.
 */
void KickCmd::generate_response(User* user, std::string const response) 
{
    user->add_response(new Response(response));
}
