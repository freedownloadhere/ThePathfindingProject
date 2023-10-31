#pragma once

#include <jni.h>
#include <windows.h>
#include <thread>
#include <memory>

#include "../Minecraft/Minecraft.h"
#include "../Pathfinder/Pathfinder.h"

namespace tpp::instance
{
	bool initialize();
	void run();

	bool init{ false };
}