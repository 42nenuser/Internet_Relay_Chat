#include "JoinCmd.hpp"

void JoinCmd::set_channel_name(std::string _channel_name)
{
    this->channel_name = _channel_name;
}

std::string JoinCmd::get_channel_name() const
{
    return channel_name;
}

void JoinCmd:: set_dynamic_user_data(std::string _user_password)
{
    this->dynamic_user_data = _user_password;
    
}

std::string JoinCmd:: get_dynamic_user_data()const
{
    return dynamic_user_data;
}

void JoinCmd::create_channel(Context **context)
{
    std::string channel = (*context)->to_lower(get_channel_name());
    Channel *new_channel = new Channel(channel);
    (*context)->channels->insert(std::make_pair(channel, new_channel));
}

bool JoinCmd::is_valid_channel_name(std::string channel_name)
{
    if(channel_name.empty()||channel_name[0] != '#')
        return(false);
    for(size_t i = 1; i < channel_name.size(); i++)
    {
        if(channel_name[i] == 32 || (channel_name[i] >= 9 && \
        channel_name[i] <= 13))
            return(false);
    }
    return true;
}

void JoinCmd:: add_creator_to_channel(Context **context) 
{
    Request* req = (*context)->request;
    std::string name_channel = (*context)->to_lower(get_channel_name());
    std::map<std::string, Channel *>::iterator it = (*context)->channels->find((channel_name));
    Channel *channel = it->second;
    User* user = (*context)->users->find(req->get_fd())->second;
    User* bot = (*context)->users->find((*context)->server_info->fd)->second;
    channel->add_user_to_channel(user);
    channel->add_user_to_channel(bot);
    channel->make_user_operator(bot->get_nickname());
}

const char* JoinCmd::user_status_prefix(User *user,Context *context, std::string channel_name)
{
    if (context->is_operator(user,channel_name))
        return "@";
    return "";
}

std::string JoinCmd::get_members_string(Context *context, Channel *channel, User *user)
{
    std::string members;
    std::vector<User *>users = channel->get_users();
    std::string channel_name = get_channel_name();
    for(size_t i = 0; i < users.size(); i++)
    {
        if(user->get_nickname() != users[i]->get_nickname())
            members += user_status_prefix(users[i], context, channel_name) + users[i]-> get_nickname() + " ";
    }
    return(members);
}

void JoinCmd::inform_operators_that_another_user_join_to_channel(User *user, Context *context, std::string &channel_name)
{
    std::string operator_res;
    std::map<std::string, Channel*>::iterator it = context->is_exist_channel(channel_name);
    Channel *channel = it->second;
    std::vector<User *> users = channel->get_users();
    for(size_t i = 0; i < users.size(); i++)
    {
        if(users[i]->get_nickname() != user->get_nickname())
            generate_response(users[i],rpl::join_channel(*user, channel_name));
    }
}

void JoinCmd::parse_request(std::string request)
{
    std::string channel_name;
    std::string dynamic_user_data;
    size_t i = 0;
    for(; i < request.size() && request[i] != 32; i++)
        channel_name += request[i];
    i++;    
    for(; i < request.size() && request[i] != 32; i++)
        dynamic_user_data += request[i];
    set_dynamic_user_data(dynamic_user_data);     
    set_channel_name(channel_name);
}

void JoinCmd::join_channel(Context *context, User *user, Channel *channel)
{
    std::string channel_name = get_channel_name();
    const char* user_status = user_status_prefix(user, context, channel_name);
    std::string members  = get_members_string(context, channel,user);
    channel->add_user_to_channel(user);
    generate_response(user, rpl::join_channel(*user, channel_name));
    generate_response(user, rpl::reply_topic(*user, channel_name,channel->get_topic()));
    if(!channel->get_topic().empty())
    {
        double time = channel->get_time();
		std::string time_convert = context->convert_double_to_string(time);
		User *setter_user = channel->get_topic_setter_user();
		generate_response(user,rpl::display_topic_setter(*setter_user,*user,get_channel_name(), time_convert));
    }
    generate_response(user, rpl::reply_names(*user, channel_name, user_status,members));
    generate_response(user, rpl::reply_end_of_names(*user, channel_name));
    generate_response(user, rpl::reply_channel_mode_is(*user, channel_name));
    inform_operators_that_another_user_join_to_channel(user, context, channel_name);
}

void JoinCmd::add_new_channel(Context *context,std::map<std::string, Channel *>::iterator it, User *user)
{
    std::string channel_name = get_channel_name();
    create_channel(&context);
    add_creator_to_channel(&context);
    it = context->is_exist_channel(channel_name);
    Channel *channel = it->second;
    const char* user_status = user_status_prefix(user, context, channel_name);
    std::string members  = get_members_string(context, channel,user);
    generate_response(user, rpl::join_channel(*user,channel_name));
    generate_response(user, rpl::reply_names(*user, channel_name, user_status, members));
    generate_response(user, rpl::reply_end_of_names(*user, channel_name));
    generate_response(user, rpl::reply_channel_mode_is(*user, channel_name));
}

void JoinCmd::handle_invitation_status(Channel *channel, User *user, std::string channel_name, Context *context)
{
    if(channel->get_only_invite())
    {
        if(channel->get_has_invited() == true && channel->is_user_invited(user->get_nickname()))
        {
            join_channel(context,user,channel);
            channel->remove_user_from_invite_list(user->get_nickname());
        }
        else if(channel->get_user_password_status() == true && !channel->is_password_correct(dynamic_user_data))
           generate_response(user,rpl::reply_password_incorrect(*user, channel_name));
        else
            generate_response(user,rpl::reply_invite_only(*user,channel_name));
        channel->set_user_password_status(false);
        channel->has_set_channel_user_limit(false);    
    }
}
    
void JoinCmd:: handle_password_verification(Channel *channel,User *user, std::string channel_name, Context *context)
{
    if(!channel->get_has_channel_user_limit())
    {
        if(!channel->is_password_correct(dynamic_user_data))
            generate_response(user,rpl::reply_password_incorrect(*user, channel_name));
        else
            join_channel(context,user,channel);
    }
    else
    {
        int number_channel_users_limit = channel->get_number_channel_users_limit();
        int number_users_on_channel = (int)channel->get_users().size();
        if(channel->is_password_correct(dynamic_user_data) && number_channel_users_limit <= number_users_on_channel)
            generate_response(user, rpl::reply_users_limit(*user,channel_name));
        else if(channel->is_password_correct(dynamic_user_data) && number_channel_users_limit > number_users_on_channel)
            join_channel(context,user, channel);   
        else
            generate_response(user,rpl::reply_password_incorrect(*user, channel_name));
    }
}

void JoinCmd::handle_user_limit_check(Channel*channel, User *user, std::string channel_name, Context *context)
{
    if(channel->get_number_channel_users_limit() <= (int)(channel->get_users().size()))
        generate_response(user, rpl::reply_users_limit(*user,channel_name));
    else
        join_channel(context,user, channel);
}

void JoinCmd::handle_user_join_request(User *user, std::string channel_name ,Context *context)
{
    std::map<std::string, Channel *>::iterator it = context->is_exist_channel(channel_name);
    if(it != context->channels->end())
    {
        Channel *channel = it->second;
        std::vector<User *> users = channel->get_users();
        std::vector<std::string> list = channel->get_invite_list();
        if (std::find(users.begin(), users.end(), user) == users.end())
        {
            if(channel->get_has_invited() && channel->is_user_invited(user->get_nickname()))
            {
                channel->remove_user_from_invite_list(user->get_nickname());
                join_channel(context,user,channel);
            }
            else if(channel->get_has_channel_user_limit() || channel->get_user_password_status() || channel->get_only_invite())
            {
                handle_invitation_status(channel,user,channel_name, context);
                if((channel->get_user_password_status() == true))
                    handle_password_verification(channel,user,channel_name, context);
                else if(channel->get_has_channel_user_limit()== true)    
                    handle_user_limit_check(channel, user,channel_name,context);
            }
            else
                join_channel(context,user,channel);
        }
    }
    else
        add_new_channel(context,it,user);
    
}

void JoinCmd::execute(Context* context)
{
    Request* req = context->request;
    User* user = context->users->find(req->get_fd())->second;
    
    if (!user->is_authenticated() || !user->is_registered()) 
        generate_response(user, rpl::unregistered());
    else
    {
        parse_request(context->request->get_options());
        if (!is_valid_channel_name(channel_name))
            generate_response(user, rpl::no_such_channel(*user, get_channel_name()));
        else
            handle_user_join_request(user,channel_name,context);
    }
}

void JoinCmd::generate_response(User* user, std::string const response) 
{
	user->add_response(new Response(response));
}
