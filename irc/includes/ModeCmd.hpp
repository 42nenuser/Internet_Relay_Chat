#include "ICommand.hpp"

class ModeCmd : public ICommand
{
    private:
        std::string channel_name;
        std::string option_mode;
        std::string authentication_info;
    public:
        void execute(Context *context);
        void generate_response(User *user, std::string const);
        void parse_request(std::string & request);
        void set_option_mode(std::string);
        void set_authentication_info(std::string);
        std::string get_authentication_info()const;
        void set_channel_name(std::string);
        std::string get_channel_name()const;
        std::string get_option_mode()const;
        bool check_is_user_operator(std::string nickname, Channel *channel);
        void handle_invite_only(Channel *channel, std::string channel_name, User *user, std::string option_mode);
        void handle_gives_and_takes_operators(Channel *channel, User *user, Context *contex);
        void update_user_status(Channel *channel, User *user);
        void notify_topic_rules_changed(Channel *channel, User *user);
        void manage_topic_command_permissions(Channel *channel, User *user);
        void manage_channel_password(Channel *channel, User *user);
        void inform_users_of_authentication_change(Channel *channel, User *user);
        void manage_channel_user_limit(Channel *channel, User *user);
        User *found_user(std::string nickname, Context *context);
        bool is_within_int_range(std::string str);
};
