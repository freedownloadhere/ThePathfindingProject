#include "CommandInterface.h"

using namespace tpp;

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
	if (!this->mInit)
		return;

	this->mInstance->minecraft->chat->sendMessageToPlayer(
		"The loop is being entered successfully\n"
	);
}