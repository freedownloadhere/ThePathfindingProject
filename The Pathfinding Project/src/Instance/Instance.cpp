#include "Instance.h"

using namespace tpp;
using namespace std::literals::chrono_literals;

Instance::Instance()
{
	std::cout << "=================================================== (Start of initialization)\n";

	this->m_init = this->init();

	std::cout << "=================================================== (Client status)\n";
	if (!this->m_init)
		std::cout << "[-] Status: not initialized\n";
	else
		std::cout << "[+] Status: initialized\n";
	std::cout << "=================================================== (End of initialization)\n\n";

	if (this->m_init)
	{
		std::cout << "[Welcome] Welcome to The Pathfinding Project!\n";
		std::cout << "This project is made by FreeDownloadHere, with passion and love :)\n";
		std::cout << "Keep in mind this is still in its early stages and prone to bugs.\n";
		std::cout << "Make sure to report any issues on the Github repo!\n\n";
		std::cout << "[Pathfinder] To make a path from your position to (x, y ,z), type in chat:\n";
		std::cout << ".tpp goto <x> <y> <z> -setblock <blockName>\n\n";
		std::cout << "Arguments:\nx, y, z = numbers (coordinates);\nblockName = string (e.g. stone)\n\n";
		std::cout << "[Exit] To exit without crashing, press Numpad key 0!\n";
	}
}

bool Instance::init()
{
	this->minecraft = std::make_shared<Minecraft>();
	if (this->minecraft == nullptr || !this->minecraft->isInit())
		return false;

	this->pathfinder = std::make_unique<Pathfinder>(this->minecraft);
	if (this->pathfinder == nullptr || !this->pathfinder->isInit())
		return false;

	Command::init(this->pathfinder);

	return true;
}

void Instance::processInput()
{
	std::string lastMessage{ this->minecraft->chat->getLatestChatMessage() };
	size_t commandStart{ lastMessage.find(".tpp")};

	if (commandStart != std::string::npos)
	{
		this->minecraft->chat->sendMessageToPlayer("§7A command is being processed. ");
		Command command(lastMessage.substr(commandStart));
	}
}

void Instance::run()
{
	while (!GetAsyncKeyState(VK_NUMPAD0))
	{
		this->processInput();
		std::this_thread::sleep_for(200ms);
	}
}