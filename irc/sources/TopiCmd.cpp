#include "TopiCmd.hpp"

void TopiCmd::set_channel_name(std::string _channel_name)
{
    this->channel_name = _channel_name;
}

void TopiCmd::set_topic(std::string _topic)
{
    this->topic = _topic;
}

std::string TopiCmd::get_channel_name() const
{
    return(channel_name);
}

std::string TopiCmd::get_topic()const 
{
    return (topic);
}

void TopiCmd::parse_request(std::string &request)
{
    std::string channel_name;
    std::string topic;
        
    size_t i = 0;
    for(; i < request.size() && request[i] != 32 ; i++)
        channel_name += request[i]; 
    i++;
    for(; i < request.size(); i++)
        topic += request[i];
    set_channel_name(channel_name);
    set_topic(topic);   
}

void TopiCmd::inform_all_users(Channel *channel,User *user ,std::string channel_name, std::string topic)
{
    std::vector<User *> users = channel->get_users();
    for(size_t i = 0; i < users.size(); i++)
    {
        if(users[i]->get_nickname() != user->get_nickname())
            generate_response(users[i],rpl::reply_set_topic(*user, channel_name, topic));
    }
}

std::string TopiCmd::convert_double_to_string(double num)
{
	std::stringstream ss;
	ss << std::fixed << num;
	return(ss.str());
}

void TopiCmd::set_channel_topic(Channel *channel, User *user, std::string topic, std::string channel_name)
{
	std::time_t current_time = std::time(nullptr);
	channel->set_time(current_time);
	generate_response(user,rpl::reply_set_topic(*user,channel_name ,topic));
	inform_all_users(channel,user,channel_name, topic);
	channel->set_topic_setter_user(user);
	channel->set_topic(topic);
}

void TopiCmd::handle_empty_topic(Channel *channel, User *user)
{
	if(!channel->get_topic().empty())
	{
		double time = channel->get_time();
		std::string time_convert = convert_double_to_string(time);
		User *setter_user = channel->get_topic_setter_user();
		generate_response(user,rpl::display_user_topic(*user, get_channel_name(), channel->get_topic()));
		generate_response(user,rpl::display_topic_setter(*setter_user,*user,get_channel_name(), time_convert));
	}
	else
		generate_response(user,rpl::no_topic_is_set(*user,channel_name));

}

void TopiCmd::execute(Context *context)
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
        std::map<std::string, Channel *>::iterator it = context->is_exist_channel(channel_name);
        if(context->is_user_on_that_channel(user->get_nickname(), channel_name))
        {
            Channel *channel = it->second;
            if((context->is_operator(user,channel_name) || channel->get_change_topic() == true) && !topic.empty())
				set_channel_topic(channel,user,get_topic(), channel_name);
            else if(get_topic().empty())
				handle_empty_topic(channel, user);
            else
                generate_response(user,rpl::reply_you_are_not_channel_operator(*user,channel_name));
        }
        else 
        {
            if(it == context->channels->end() )
                generate_response(user, rpl::no_such_channel(*user,channel_name));
            else
                generate_response(user, rpl::you_are_not_on_channel(*user, channel_name));
        }
    }
}

void TopiCmd::generate_response(User *user, std::string response)
{
    user->add_response(new Response(response));
}