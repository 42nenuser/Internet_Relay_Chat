#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

# include <iostream>
# include "Request.hpp"
# include "ICommand.hpp"
# include "NickCmd.hpp"
# include "PassCmd.hpp"
# include "UserCmd.hpp"
# include "JoinCmd.hpp"
# include "KickCmd.hpp"
# include "PrivmsgCmd.hpp"
# include "InviteCmd.hpp"
# include "ModeCmd.hpp"
# include "TopiCmd.hpp"
# include "QuitCmd.hpp"

class RequestHandler {
	private:
		ICommand* identify_cmd(Request*);
	public:
		void handle_request(Context*);
		RequestHandler();
};

#endif