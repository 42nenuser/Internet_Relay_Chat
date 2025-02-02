#include "InviteCmd.hpp"

void InviteCmd::set_channelname(std::string name_ch)
{
    this->channel_name = name_ch;
}

void InviteCmd::set_nickname(std::string nickname)
{
    this->nickname = nickname;
}

std::string InviteCmd::get_channel_name()
{
    return channel_name;
}

std::string InviteCmd::get_nickname()
{
    return nickname;
}

void InviteCmd::parse_request(std::string &request)
{
    std::string channel_name;
    std::string nickname;
        
    size_t i = 0;
    for(; request[i] != ' ' && i < request.size() ; i++)
        nickname += request[i]; 
    i++;
    for(; i < request.size() && request[i] != ' '; i++)
        channel_name += request[i];
    set_channelname(channel_name);
    set_nickname(nickname);       
}

bool InviteCmd::is_exist_user(std::string nickname, Context *context)
{
    std::unordered_map<int , User*> ::iterator it;
    for(it = context->users->begin(); it != context->users->end(); it++)
    {
        if((it)->second->get_nickname() == nickname)
            return(true);
    }
    return(false);
}

User * InviteCmd::found_user(std::string nickname, Context *context)
{
    std::unordered_map<int , User*> ::iterator it;
    for(it = context->users->begin(); it != context->users->end(); it++)
    {
        if((it)->second->get_nickname() == nickname)
            return(it->second);
    }
    return(NULL);
}

void InviteCmd::invite_user(User *user,std::string nickname, std::string channel_name, Context *context)
{
    User *user_invite = found_user(nickname, context);
    if(user_invite != NULL)
        generate_response(user_invite, rpl::reply_invite_user(*user, nickname, channel_name));
}

void InviteCmd::execute(Context* context)
{
    Request *req = context->request; 
    std::string request = req->get_options();
    User *user = context->users->find(req->get_fd())->second;
    if (!user->is_authenticated() || !user->is_registered()) 
        generate_response(user, rpl::unregistered());
    else 
    {
        parse_request(request);  
        std::string channel_name = get_channel_name();
        std::map<std::string, Channel *>::iterator it = context->is_exist_channel(channel_name);
        Channel *channel = it->second;
        if(it == context->channels->end() || ((!is_exist_user(get_nickname(),context))))
            generate_response(user, rpl::no_such_nick(*user, get_nickname()));
        else if(context->is_user_on_that_channel(user->get_nickname(),channel_name))
        {
            if(context->is_operator(user,channel_name))
            {
                if(context->is_user_on_that_channel(get_nickname(),channel_name) == false)
                {
                    channel->set_has_invited(true);
                    channel->set_invite_list(get_nickname());
                    generate_response(user, rpl::reply_exist_user_and_channel(*user, get_nickname(), channel_name));
                    invite_user(user, get_nickname(), channel_name, context);
                } 
                else
                    generate_response(user, rpl::is_already_on_channel(*user,get_nickname(),channel_name));
            }
            else
                generate_response(user,rpl::reply_you_are_not_channel_operator(*user,channel_name));
        }
        else 
            generate_response(user, rpl::you_are_not_on_channel(*user, channel_name));
    }
}

void InviteCmd::generate_response(User *user, std::string const response)
{
    user->add_response(new Response(response));
}