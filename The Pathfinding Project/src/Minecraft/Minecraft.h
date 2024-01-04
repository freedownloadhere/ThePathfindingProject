#pragma once

#include <memory>

#include "Player/Player.h"
#include "World/World.h"
#include "Chat/Chat.h"
#include "../JNI/JNI.h"

namespace tpp::minecraft
{
	enum result
	{
		OK = 0,
		PLAYER_FAILED = 1,
		WORLD_FAILED = 2,
		CHAT_FAILED = 3,
		GET_INSTANCE_FAILED = 4
	};

	result initialize();

	inline bool init{ false };
}