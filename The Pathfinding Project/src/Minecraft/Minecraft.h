#pragma once

#include <memory>

#include "Player/Player.h"
#include "World/World.h"
#include "Chat/Chat.h"
#include "../JNI/JNI.h"

namespace tpp::minecraft
{
	bool initialize();

	jclass mcClass{ nullptr };
	jobject mcClassInstance{ nullptr };

	bool init{ false };
}