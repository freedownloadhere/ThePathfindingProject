#include "Minecraft.h"

using namespace tpp;

Minecraft::Minecraft()
{
	this->mInit = this->init();

	if (!this->mInit)
		std::cout << "	[-] An error occured while initializing Minecraft\n";
	else
		std::cout << "[+] Successfully initialized Minecraft\n";
}

bool Minecraft::init()
{
	JavaVM* jvm{ nullptr };

	if (JNI_GetCreatedJavaVMs(&jvm, 1, nullptr) != 0)
	{
		std::cout << "[-] Failed to get created Java VMs!\n";
		return false;
	}

	jvm->AttachCurrentThread((void**)&this->env, nullptr);
	if (this->env == nullptr)
	{
		std::cout << "[-] Failed to attach current JVM thread!\n";
		return false;
	}

	this->mcClass = tpp::getClass(this->env, "net/minecraft/client/Minecraft");
	if (this->mcClass == nullptr)
	{
		std::cout << "[-] Failed to find class Minecraft!\n";
		std::cout << "[INFO] Are you using MC Forge 1.8.9?\n";
		return false;
	}

	jfieldID fieldID{ env->GetStaticFieldID(this->mcClass, "field_71432_P", "Lnet/minecraft/client/Minecraft;") };
	if (fieldID == nullptr)
	{
		std::cout << "[-] Failed to get static field ID theMinecraft!\n";
		return false;
	}

	this->mcClassInstance = env->GetStaticObjectField(this->mcClass, fieldID);
	if (this->mcClassInstance == nullptr)
	{
		std::cout << "[-] Failed to get static object field theMinecraft!\n";
		return false;
	}

	this->player = std::make_unique<Player>(
		env,
		mcClass,
		mcClassInstance
	);
	if (this->player == nullptr || !this->player->isInit())
	{
		return false;
	}

	this->world = std::make_unique<World>(
		env,
		mcClass,
		mcClassInstance
	);
	if (this->world == nullptr || !this->world->isInit())
	{
		return false;
	}

	this->chat = std::make_unique<Chat>(
		env,
		mcClass,
		mcClassInstance,
		this->player->getEntityPlayerSPClass(),
		this->player->getMcThePlayerInstance()
	);
	if (this->chat == nullptr || !this->chat->isInit())
	{
		return false;
	}

	return true;
}

bool Minecraft::isInit()
{
	return this->mInit;
}