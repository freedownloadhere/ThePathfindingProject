#include "Minecraft.h"

using namespace tpp;

minecraft::result minecraft::initialize()
{
	player::initialize();
	if (!player::init) return PLAYER_FAILED;

	world::initialize();
	if (!world::init) return WORLD_FAILED;

	chat::initialize();
	if (!chat::init) return CHAT_FAILED;

	init = true;
	return OK;
}