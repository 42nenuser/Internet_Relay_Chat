#ifndef NICKCMD_HPP
# define NICKCMD_HPP
# include "ICommand.hpp"

class NickCmd : public ICommand {
	private:
		bool is_duplicate(Context*);
		bool is_nickname_valid(Context*);
		bool has_invalid_char(Context*);
	public:
		void execute(Context*);
		void generate_response(User*, std::string const);
};

#endif