#pragma once

#include <windows.h>
#include <thread>
#include <memory>

#include "../JNI/JNI.h"
#include "../Minecraft/Minecraft.h"
#include "../Pathfinder/Pathfinder.h"

namespace tpp::instance
{
	bool initialize();
	void run();

	inline bool init{ false };
}