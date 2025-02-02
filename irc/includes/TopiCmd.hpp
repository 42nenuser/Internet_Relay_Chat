#pragma once

#include "ICommand.hpp"
#include <ctime>
#include <sstream>
#include <cmath>
#include <iomanip>

class TopiCmd : public ICommand
{
    private:
        std::string channel_name;
        std::string topic;
    public :
        void execute(Context*);
		void generate_response(User*, std::string const);
        void parse_request(std::string &);
        void set_channel_name(std::string);
        void set_topic(std::string);
        std::string get_channel_name()const;
        std::string get_topic()const;
        void inform_all_users(Channel *channel,User *user ,std::string channel_name, std::string topic);
        void set_channel_topic(Channel *channel, User *user, std::string topic, std::string channel_name);
        void handle_empty_topic(Channel *channel, User *user);
        std::string convert_double_to_string(double num);
};