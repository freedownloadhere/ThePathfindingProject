#include "CommandInterface.h"

using namespace tpp;
using namespace std::literals::chrono_literals;

CommandInterface::CommandInterface(const std::shared_ptr<Instance>& instance)
{
	this->mInit = this->init(instance);
}

bool CommandInterface::init(const std::shared_ptr<Instance>& instance)
{
	this->mInstance = instance;
	if (this->mInstance == nullptr || !this->mInstance->isInit())
		return false;

	return true;
}

void CommandInterface::enterLoop()
{
	while (!GetAsyncKeyState(VK_NUMPAD0))
	{
		if (!this->mInit)
			return;

		std::cin >> this->mCmdName;
		std::getline(std::cin, this->mCmdArgs);

		this->mCmdArgs.erase(this->mCmdArgs.begin()); // Delete the one whitespace... lol

		if (this->mCmdName == "goto") this->mCmdResult = this->cmdGoto();
		else if (this->mCmdName == "makepath") this->mCmdResult = this->cmdMakePath();
		else if (this->mCmdName == "print") this->mCmdResult = this->cmdPrint();
		else if (this->mCmdName == "send") this->mCmdResult = this->cmdSend();
		else if (this->mCmdName == "wait") this->mCmdResult = this->cmdWait();

		else if (this->mCmdName == "end") return;

		else
		{
			std::cout << "[Command] \"" << this->mCmdName << "\" is not a valid command name.\n";
			continue;
		}

		if (this->mCmdResult == true)
			std::cout << "[Command] Executed " << this->mCmdName << " successfully.\n";
		else
			std::cout << "[Command] Failed to execute " << this->mCmdName << ".\n";
	}
}

bool CommandInterface::cmdGoto()
{
	std::istringstream ss(this->mCmdArgs);

	Vector3 coordinates{ tpp::nullvector };
	std::string blockType{ "stone" };

	if (!(ss >> coordinates.x))
	{
		std::cout << "[Command] goto::x parameter is invalid\n";
		return false;
	}

	if (!(ss >> coordinates.y))
	{
		std::cout << "[Command] goto::y parameter is invalid\n";
		return false;
	}

	if (!(ss >> coordinates.z))
	{
		std::cout << "[Command] goto::z parameter is invalid\n";
		return false;
	}

	ss >> blockType;

	return this->mInstance->pathfinder->goTo(coordinates, blockType);
}

bool CommandInterface::cmdMakePath()
{
	std::this_thread::sleep_for(5s);

	std::istringstream ss(this->mCmdArgs);

	Vector3 start{ tpp::nullvector }, end{ tpp::nullvector };
	std::string blockType{ "stone" }, blockData{ "" };

	if (!(ss >> start.x))
	{
		std::cout << "[Command] makepath::x1 parameter is invalid\n";
		return false;
	}

	if (!(ss >> start.y))
	{
		std::cout << "[Command] makepath::y1 parameter is invalid\n";
		return false;
	}

	if (!(ss >> start.z))
	{
		std::cout << "[Command] makepath::z1 parameter is invalid\n";
		return false;
	}

	if (!(ss >> end.x))
	{
		std::cout << "[Command] makepath::x2 parameter is invalid\n";
		return false;
	}

	if (!(ss >> end.y))
	{
		std::cout << "[Command] makepath::y2 parameter is invalid\n";
		return false;
	}

	if (!(ss >> end.z))
	{
		std::cout << "[Command] makepath::z2 parameter is invalid\n";
		return false;
	}

	ss >> blockType >> blockData;
	blockType += " " + blockData;

	return this->mInstance->pathfinder->makePath(start, end, blockType);
}

bool CommandInterface::cmdPrint()
{
	return this->mInstance->minecraft->chat->sendMessageToPlayer(this->mCmdArgs);
}

bool CommandInterface::cmdSend()
{
	return this->mInstance->minecraft->chat->sendMessageFromPlayer(this->mCmdArgs);
}

bool CommandInterface::cmdWait()
{
	int waitFor{ 0 };

	try
	{
		waitFor = std::stoi(this->mCmdArgs);
	}
	catch (...)
	{
		std::cout << "[Command] wait::waitFor parameter is invalid\n";
		return false;
	}

	std::cout << "[Command] Waiting for " << waitFor << " ms...\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(waitFor));

	return true;
}