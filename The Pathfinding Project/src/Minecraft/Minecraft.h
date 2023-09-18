#pragma once

#include <jni.h>
#include <memory>

#include "Player/Player.h"
#include "World/World.h"
#include "Chat/Chat.h"

namespace tpp
{
	class Minecraft
	{
	public:
		Minecraft();

		std::unique_ptr<Player> player{ nullptr };
		std::unique_ptr<World> world{ nullptr };
		std::unique_ptr<Chat> chat{ nullptr };

		bool isInit();

	private:
		jclass mcClass{ nullptr };
		jobject mcClassInstance{ nullptr };

		bool m_init{ false };
		bool init();
		JNIEnv* env{ nullptr };
	};
}