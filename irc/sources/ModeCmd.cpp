#include "ModeCmd.hpp"

void ModeCmd::set_channel_name(std::string _channel_name)
{
    this->channel_name = _channel_name;
}

void ModeCmd::set_option_mode(std::string _option_mode)
{
    this->option_mode = _option_mode;
}

std::string  ModeCmd::get_channel_name() const
{
    return channel_name;
}

std::string ModeCmd::get_option_mode()const
{
    return option_mode;
}

void ModeCmd::set_authentication_info(std::string _authentication_info)
{
    this->authentication_info = _authentication_info;
}

std::string ModeCmd::get_authentication_info()const
{
    return authentication_info;
}

void ModeCmd::parse_request(std::string & request)
{
    std::string channel_name;
    std::string option_mode;
    std::string authentication_info;
    size_t i = 0;
    for(; i < request.size() && request[i] != 32; i++)
        channel_name +=request[i];
    i++;
    for(;  i < request.size() && request[i] != 32; i++)
        option_mode += request[i];
    i++;
    for(; i <request.size() && request[i] != 32; i++)
        authentication_info +=request[i];
    set_authentication_info(authentication_info);    
    set_channel_name(channel_name);
    set_option_mode(option_mode);    
}

bool ModeCmd::check_is_user_operator(std::string nickname, Channel *channel)
{
    std::set<std::string>operators = channel->get_operators();
    std::set<std::string>::iterator it_set = operators.find(nickname);
    if(it_set != operators.end())
        return(true);
    return(false);
}

void ModeCmd::handle_invite_only(Channel *channel, std::string channel_name, User *user, std::string option_mode)
{
    if(option_mode == "+i" && channel->get_only_invite() == false)
    {
        generate_response(user,rpl::reply_rules_channel(*user,channel_name, option_mode));
        channel->set_only_invite(true);
    }
    else if(option_mode == "-i" && channel->get_only_invite() == true)
    {
        generate_response(user,rpl::reply_rules_channel(*user,channel_name, option_mode));
        channel->set_only_invite(false);
    }
}

void ModeCmd::update_user_status(Channel *channel, User *user)
{
    std::vector<User *> users = channel->get_users();
    for(size_t i = 0;i < users.size(); i++)
    {
        if(users[i]->get_nickname() != user->get_nickname())
        {
            generate_response(users[i], rpl::notify_operator_privilege_change(*user,get_channel_name(), get_option_mode(),get_authentication_info()));
        }
    }
}

User * ModeCmd::found_user(std::string nickname, Context *context)
{
    std::unordered_map<int , User*> ::iterator it;
    for(it = context->users->begin(); it != context->users->end(); it++)
    {
        if((it)->second->get_nickname() == nickname)
            return(it->second);
    }
    return(NULL);
}

void ModeCmd::handle_gives_and_takes_operators(Channel *channel, User *user, Context *context)
{
    if (get_authentication_info() == context->users->find(context->server_info->fd)->second->get_nickname())
        return ;
  if(found_user(get_authentication_info(),context))
  {
        if(context->is_user_on_that_channel(get_authentication_info(), get_channel_name()))
        {
            if(option_mode =="+o" && !check_is_user_operator(get_authentication_info(),channel))
            {
                channel->make_user_operator(get_authentication_info());
                generate_response(user, rpl::notify_operator_privilege_change(*user,get_channel_name(), get_option_mode(),get_authentication_info()));
                update_user_status(channel,user);
            }
            else if(option_mode == "-o" && check_is_user_operator(get_authentication_info(),channel))
            {
                channel->revoke_operator_status(get_authentication_info());
                generate_response(user,rpl::notify_operator_privilege_change(*user,channel_name,option_mode,get_authentication_info()));
                update_user_status(channel, user);
            }
        }
        else
            generate_response(user,rpl::reply_are_not_on_channel(*user,get_authentication_info(),channel_name));
  }
  else
    generate_response(user, rpl::no_such_nick(*user,get_authentication_info()));
}
void ModeCmd::notify_topic_rules_changed(Channel *channel, User *user)
{
    std::vector<User *> users = channel->get_users();
    for(size_t i = 0; i < users.size(); i++)
    {
        if(users[i]->get_nickname() != user->get_nickname())
            generate_response(users[i],rpl::reply_rules_channel(*user,get_channel_name(),get_option_mode()));
    }
}

void ModeCmd::inform_users_of_authentication_change(Channel *channel, User *user)
{
    std::vector<User *> users = channel->get_users();
    for(size_t i = 0; i < users.size(); i++)
    {
        if(users[i]->get_nickname() != user->get_nickname())
            generate_response(users[i],rpl::reply_set_authentication_channel(*user,get_channel_name(),get_option_mode(),get_authentication_info()));
    }
    
}

void ModeCmd::manage_topic_command_permissions(Channel *channel, User *user)
{
    if(get_option_mode() == "-t" &&  channel->get_has_seted_topic() == false)
    {
        generate_response(user,rpl::reply_rules_channel(*user,get_channel_name(),get_option_mode()));
        notify_topic_rules_changed(channel,user);
        channel->allow_user_to_change_topic(true);
        channel->has_seted_topic(true);
    }
    else if(get_option_mode() == "+t" &&  channel->get_has_seted_topic() == true)
    {
        generate_response(user,rpl::reply_rules_channel(*user,get_channel_name(),get_option_mode()));
        notify_topic_rules_changed(channel,user);
        channel->allow_user_to_change_topic(false);
        channel->has_seted_topic(false);
    }
}

void ModeCmd::manage_channel_password(Channel *channel, User *user)
{
    if((get_option_mode() == "+k" || get_option_mode() == "-k") && get_authentication_info().empty())
    {
        generate_response(user,rpl::not_enough_params(*user));
        return;
    }
    if(get_option_mode() == "+k" )
    {
        if(channel->get_user_password_status() == false)
        {
            generate_response(user,rpl::reply_set_authentication_channel(*user,get_channel_name(),get_option_mode(),get_authentication_info()));
            inform_users_of_authentication_change(channel, user);
            channel-> set_user_password_status(true);
            channel->set_password(get_authentication_info());
        }
        else if(channel->get_user_password_status() == true)
            generate_response(user,rpl::channel_key_already_set(*user, get_channel_name()));    
    }
    if(get_option_mode() == "-k")
    {
        if(channel->get_user_password_status() == false || channel->is_password_correct(get_authentication_info()) == false)
            generate_response(user,rpl::channel_key_already_set(*user, get_channel_name()));
        else if(channel->get_user_password_status() == true && channel->is_password_correct(get_authentication_info()) == true)
        {
            generate_response(user,rpl::reply_set_authentication_channel(*user,get_channel_name(),get_option_mode(),get_authentication_info()));
            inform_users_of_authentication_change(channel, user);
            channel->set_user_password_status(false); 
        }
    }
    
}

bool ModeCmd::is_within_int_range(std::string str) 
{
    std::string max_int = "2147483647";
    if(str.size() > max_int.size())
        return false;
    else if(str.size() == max_int.size())
    {
        for(size_t i = 0; i < str.size(); i++)
        {
            if(str[i] > max_int[i])
                return false;
        }
    }
    return true;
}

void  ModeCmd::manage_channel_user_limit(Channel *channel, User *user)
{
    if((get_option_mode() == "+l"  ) && get_authentication_info().empty())
        generate_response(user,rpl::not_enough_params(*user));
    else if(get_option_mode() == "+l")
    { 

        if(!is_within_int_range(get_authentication_info()))
            return;
        int number = atoi(get_authentication_info().c_str());
        
        if(number > 0)
        {
            if(channel->get_has_channel_user_limit() == false)
            {
                generate_response(user, rpl::reply_set_authentication_channel(*user,get_channel_name(),get_option_mode(),get_authentication_info()));
                inform_users_of_authentication_change(channel, user);
                channel->set_number_channel_users_limit(number);
                channel->has_set_channel_user_limit(true); 
            }
            else
            {
                if(channel->number_users_limit_change(number))
                {
                    generate_response(user, rpl::reply_set_authentication_channel(*user,get_channel_name(),get_option_mode(),get_authentication_info()));
                    inform_users_of_authentication_change(channel, user);
                    channel->set_number_channel_users_limit(number);
                }
            }
        }
    }
    else  if(get_option_mode() == "-l" )
    {
        if( !get_authentication_info().empty())
            generate_response(user,rpl::unknown_mode(*user,get_authentication_info()));
        else if(channel->get_has_channel_user_limit() == true)
        {
            generate_response(user, rpl::reply_set_authentication_channel(*user,get_channel_name(),get_option_mode(),get_authentication_info()));
            inform_users_of_authentication_change(channel, user);
            channel->has_set_channel_user_limit(false);
        }
    }
}

void ModeCmd::execute(Context *context)
{
    Request *req = context->request;
    User *user = context->users->find(req->get_fd())->second;
    if (!user->is_authenticated() || !user->is_registered()) 
        generate_response(user, rpl::unregistered());
    else
    {
        std::string request = req->get_options();
        parse_request(request);
        std::string channel_name = get_channel_name();
        std::map<std::string , Channel *>::iterator it = context->is_exist_channel(channel_name);
        if(!option_mode.empty())
        {
            if(it == context->channels->end())
                generate_response(user,rpl::no_such_channel(*user, get_channel_name()));
            else if(context->is_user_on_that_channel(user->get_nickname(), get_channel_name()))
            {
                Channel *channel = it->second;
                if(context->is_operator(user, get_channel_name()))
                {
                    if(get_option_mode() == "+o" || get_option_mode() == "-o")
                    {
                        handle_gives_and_takes_operators(channel,user, context);
                        return;
                    }
                    handle_invite_only(channel, get_channel_name(), user, get_option_mode());
                    manage_topic_command_permissions(channel,user);
                    manage_channel_password(channel,user);
                    manage_channel_user_limit(channel, user);
                }
                else if (context->is_operator(user, get_channel_name()) == false )
                    generate_response(user, rpl::reply_you_are_not_channel_operator(*user,get_channel_name()));
            }
            else
                generate_response(user,rpl::you_are_not_on_channel(*user,channel_name));
        }
    }
}

void ModeCmd::generate_response(User *user, std::string const response)
{
      user->add_response(new Response(response));
}