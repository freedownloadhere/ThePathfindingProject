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
	std::cout << "=================================================== (End of initialization)\n\n\n";
}

bool Instance::init()
{
	this->minecraft = std::make_shared<Minecraft>();

	if (this->minecraft == nullptr || !this->minecraft->isInit())
		return false;

	return true;
}

void Instance::run()
{
	this->minecraft->chat->sendMessageToPlayer("Working!\n");

	std::this_thread::sleep_for(1s);
}