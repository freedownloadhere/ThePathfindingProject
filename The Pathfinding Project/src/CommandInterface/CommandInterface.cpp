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
		else if (this->mCmdName == "echo") this->mCmdResult = this->cmdEcho();
		else if (this->mCmdName == "cout") this->mCmdResult = this->cmdCout();

		else if (this->mCmdName == "end") return;

		else
		{
			std::cout << "[Command] \"" << this->mCmdName << "\" is not a valid command name.\n";
			continue;
		}

		if (this->mCmdResult == true)
			if(this->mEcho) std::cout << "[Command] Executed " << this->mCmdName << " successfully.\n";
		else
			if(this->mEcho) std::cout << "[Command] Failed to execute " << this->mCmdName << ".\n";
	}
}

bool CommandInterface::cmdGoto()
{
	std::istringstream ss(this->mCmdArgs);
	std::string buffer{ "" };

	Vector3 coordinates{ tpp::nullvector };
	int flags{ 0 };
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

	while (ss >> buffer)
	{
		if (buffer == "+setblock")
		{
			flags += (int)MakePathFlags::SETBLOCK;
			ss >> blockType;
		}
		else if (buffer == "+safe")
		{
			flags += (int)MakePathFlags::SAFE;
		}
		else if (buffer == "+useprevcache")
		{
			flags += (int)MakePathFlags::USEPREVCACHE;
		}
		else
		{
			std::cout << "[Command] Invalid goto flag\n";
			return false;
		}
	}

	ss >> blockType;

	return this->mInstance->pathfinder->goTo(coordinates, flags, blockType);
}

bool CommandInterface::cmdMakePath()
{
	std::istringstream ss(this->mCmdArgs);
	std::string buffer{ "" };

	Vector3 start{ tpp::nullvector }, end{ tpp::nullvector };
	int flags{ 0 };
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

	while (ss >> buffer)
	{
		if (buffer == "+setblock")
		{
			flags += (int)MakePathFlags::SETBLOCK;
			ss >> blockType;
		}
		else if (buffer == "+safe")
		{
			flags += (int)MakePathFlags::SAFE;
		}
		else if (buffer == "+useprevcache")
		{
			flags += (int)MakePathFlags::USEPREVCACHE;
		}
		else
		{
			std::cout << "[Command] Invalid makepath flag\n";
			return false;
		}
	}

	return this->mInstance->pathfinder->makePath(start, end, flags, blockType);
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

	if(this->mEcho) std::cout << "[Command] Waiting for " << waitFor << " ms...\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(waitFor));

	return true;
}

bool CommandInterface::cmdEcho()
{
	if (this->mCmdArgs == "off")
	{
		this->mEcho = false;
		return true;
	}
	else if (this->mCmdArgs == "on")
	{
		this->mEcho = true;
		return true;
	}

	std::cout << "[Command] echo parameter is invalid. Valid parameters: on, off\n";
	return false;
}

bool CommandInterface::cmdCout()
{
	std::cout << this->mCmdArgs << '\n';
	return true;
}