#include "Command.h"

using namespace tpp;

Command::Command(const std::string& command)
{
	if (Command::mInit)
		this->tokenize(command);
	else
		std::cout << "[Command Failure] Could not tokenize command as the command object was not initialized\n";
}

void Command::tokenize(const std::string& command)
{
	std::cout << command << '\n';

	std::istringstream ss(command);
	std::string buffer;

	// Check whether it's actually a command
	ss >> buffer;
	if (buffer != ".tpp")
		return;

	// Get command
	ss >> buffer;

	if (buffer == "goto")
	{
		Vector3 gotoArg{ tpp::nullvector };
		bool setBlocks{ false };
		std::string blockToSet{ "stone" };

		// Get xyz args
		ss >> gotoArg.x >> gotoArg.y >> gotoArg.z;
		
		if (
			gotoArg.x == tpp::nullvector.x ||
			gotoArg.y == tpp::nullvector.y ||
			gotoArg.z == tpp::nullvector.z
			)
		{
			std::cout << "[Command Failure] goto: Expected x y z arguments\n";
			return;
		}

		// Get optional setblock arg
		ss >> buffer;

		if (buffer == "-setblock")
		{
			setBlocks = true;
			ss >> blockToSet;

			for (auto& i : blockToSet)
				if (!isalnum(i))
					i = ' ';

			this->pathfinder->moveTo(gotoArg, blockToSet);
		}
		else 
			this->pathfinder->moveTo(gotoArg);

		return;
	}

	else
	{
		std::cout << "[Command Failure] Expected a command after .tpp";
	}
}

void Command::init(const std::shared_ptr<Pathfinder>& pathfinder)
{
	Command::pathfinder = pathfinder;
	Command::mInit = true;
}