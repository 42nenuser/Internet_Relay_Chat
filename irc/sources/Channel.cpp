# include "Channel.hpp"

Channel::Channel(std::string name):name_of_channel(name){
    this->only_invite = false;
    this->has_invited = false;
    this->seted_topic = false;
    this->change_topic = false;
    this->has_set_password = false;
    this->has_set_users_limit = false;
    this->number_users_on_channel = 0;
    this->setter_user_topic = nullptr;
    this->time = 0;
}

void Channel::set_time(double _time)
{
    this->time = _time;
}

double Channel::get_time()const
{
    return time;
    
}

std::string Channel:: get_topic()const
{
    return topic;
}

void Channel::set_topic(std::string _topic)
{
    this->topic = _topic;
}

void Channel::set_topic_setter_user(User *user)
{
    this->setter_user_topic = user;
}

User * Channel::get_topic_setter_user()const
{
    return setter_user_topic;
}

bool Channel::number_users_limit_change(int number_users_in_channel)
{
    if(number_users_in_channel != get_number_channel_users_limit())
        return(true);
    return false;  
}

void Channel::set_number_channel_users_limit(int users_limit)
{
     this->number_users_on_channel = users_limit;
}

int Channel::get_number_channel_users_limit()const
{
    return  number_users_on_channel;
}
void Channel::has_set_channel_user_limit(bool set_limit_user)
{
    this->has_set_users_limit = set_limit_user;

}

bool Channel::get_has_channel_user_limit()const
{
    return has_set_users_limit;
}

void Channel::set_password(std::string _password)
{
    this->password = _password;
}

std::string Channel::get_password()const
{
    return password;
}

void Channel:: set_user_password_status(bool _has_set_password)
{
    this->has_set_password = _has_set_password;
}

bool Channel::get_user_password_status()const
{
    return has_set_password;
}

void Channel::set_only_invite(bool _only_invite)
{
    this->only_invite = _only_invite;
}

void Channel::allow_user_to_change_topic(bool _change_topic)
{
    this->change_topic = _change_topic;
}

bool Channel::get_change_topic()const
{
    return change_topic;
}

bool Channel::get_only_invite()const
{
    return only_invite;
}

void Channel::has_seted_topic(bool _topic)
{
    this->seted_topic = _topic;
}

bool Channel::get_has_seted_topic()const
{
    return seted_topic;
}

void Channel::set_has_invited(bool _has_invited)
{
    this->has_invited = _has_invited;
}

bool Channel::get_has_invited()const
{
    return has_invited;
}

void Channel::set_invite_list(std::string _nickname_invited)
{
    invite_list.push_back(_nickname_invited);
}

std::vector<std::string>& Channel::get_invite_list()
{
    return invite_list;
}

std::vector<User*>& Channel::get_users()
{
    return Users;
}

void Channel::add_user_to_channel(User *user) 
{
    if(Users.empty())
        oper.insert(user->get_nickname());
    Users.push_back(user);
}

bool Channel::is_password_correct(std::string user_input_password)
{
   if(get_password().compare(user_input_password) == 0)
        return true;
   return false;
}

void Channel:: make_user_operator(std::string nickname)
{
   for(std::vector<User *>::iterator it = Users.begin(); it != Users.end(); it++)
   {
    if((*it)->get_nickname() == nickname)
        oper.insert(nickname);

   }
}

void Channel::revoke_operator_status(std::string nickname)
{
    for(std::set<std::string> ::iterator it = oper.begin(); it != oper.end(); it++)
    {
        if((*it)== nickname)
        {
            oper.erase(it);
            break;
        }
    }
}

bool Channel:: is_exist_user(std::string nickename)
{
    std::vector<User *> users = get_users();
    for(size_t i = 0; i < users.size(); i++)
    {
        if(users[i]->get_nickname() == nickename)
            return(true);
    }
    return(false);
}

std::set<std::string> Channel::get_operators() const
{
    return(oper);
}

void Channel::kick_user(std::string nickname)
{
    for (size_t i = 0; i < Users.size(); i++)
    {
        if(Users[i]->get_nickname() == nickname)
        {
            Users.erase(Users.begin() + i);
            return;
        }
    }
}

void Channel::remove_user_from_invite_list(std::string invite_nickname)
{
    for(size_t i = 0; i < invite_list.size(); i++)
    {
        if(invite_list[i] == invite_nickname)
        {
            invite_list.erase(invite_list.begin() + i);
            return;
        }
    }
}

bool Channel::is_user_invited(std::string invite_nickname)
{
    for(size_t i = 0; i < invite_list.size(); i++)
    {
        if(invite_list[i] ==  invite_nickname)
        {
            return true; 
        }
    }
    return false;
}