#pragma once

#include <jni.h>
#include <windows.h>
#include <thread>
#include <memory>

#include "../Minecraft/Minecraft.h"
#include "../Pathfinder/Pathfinder.h"

namespace tpp
{
	class Instance final
	{
	public:
		Instance();
		std::shared_ptr<Minecraft> minecraft{ nullptr };
		//std::unique_ptr<Pathfinder> pathfinder{ nullptr };

		void run();

	private:
		bool m_init{ false };
		bool init();
	};
}