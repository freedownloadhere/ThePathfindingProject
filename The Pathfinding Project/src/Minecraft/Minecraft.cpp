#include "Minecraft.h"

using namespace tpp;

namespace tpp::minecraft
{
	static jobject mcClassInstance{ nullptr };
}

minecraft::result minecraft::initialize()
{
	minecraft::mcClassInstance = jni::get_static_obj("Minecraft", "theMinecraft");
	if (minecraft::mcClassInstance == nullptr)
	{
		std::cout << "[-] Failed to get static object field theMinecraft!\n";
		return GET_INSTANCE_FAILED;
	}

	player::initialize(
		mcClassInstance
	);
	if (!player::init) return PLAYER_FAILED;

	world::initialize(
		mcClassInstance
	);
	if (!world::init) return WORLD_FAILED;

	chat::initialize(
		mcClassInstance,
		jni::get_obj(mcClassInstance, "Minecraft", "thePlayer")
	);
	if (!chat::init) return CHAT_FAILED;

	init = true;
	return OK;
}