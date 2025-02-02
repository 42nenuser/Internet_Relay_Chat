#ifndef PASSCMD_HPP
# define PASSCMD_HPP
# include "ICommand.hpp"

class PassCmd : public ICommand {
	public:
		void execute(Context*);
		void generate_response(User*, std::string const);
};

#endif