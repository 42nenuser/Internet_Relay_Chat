#pragma once

#include "ICommand.hpp"

class InviteCmd : public ICommand
{
    private:
        std::string channel_name;
        std::string nickname;
    public:
        void execute(Context*);
        void generate_response(User*, std::string const);
        void set_channelname(std::string);
        void set_nickname(std::string nickname);
        std::string get_channel_name();
        std::string get_nickname();
        void parse_request(std::string &request);
        bool is_exist_user(std::string , Context  *);
        void invite_user(User *,std::string nickname, std::string channel_name, Context *context);
        User * found_user(std::string nickname, Context *context);
};
