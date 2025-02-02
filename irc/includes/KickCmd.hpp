# include "ICommand.hpp"

class KickCmd : public ICommand 
{
	private:
		std::string channel_name;
		std::string user_name;
	public:
		void execute(Context*);
        void generate_response(User* user, std::string const response);
		void parse_channel_and_user(std::string &request);
		void set_channelname(std::string);
		void set_username(std::string );
		std::string get_channel_name();
		std::string get_username();
		std::map<std::string, Channel *> ::iterator is_exist_channel(Context *context, std::string&);
		bool is_operator(User *user, Context *context, std::string channel_name);
		void handle_response_for_kick_user(User* user, std::string user_kicked, std::string channel_name, Channel *channel);
};