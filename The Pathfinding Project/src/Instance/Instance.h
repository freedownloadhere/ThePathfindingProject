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
		std::shared_ptr<Pathfinder> pathfinder{ nullptr };

		void run();
		bool isInit();

	private:
		bool mInit{ false };
		bool init();
	};
}