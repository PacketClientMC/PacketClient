#pragma once
#include "../../Module/Modules/Other/IRC.h"
#include "ICommand.h"
#include <sstream>

class IRCCommand : public IMCCommand {
public:
	virtual bool execute(std::vector<std::string>* args) override;
	IRCCommand();
};