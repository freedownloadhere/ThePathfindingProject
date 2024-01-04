#pragma once

#include <windows.h>
#include <thread>
#include <memory>

#include "../JNI/JNI.h"
#include "../Minecraft/Minecraft.h"
#include "../Pathfinder/Pathfinder.h"
#include "../Hooks/Hooks.h"
#include "../GUI/GUI.h"

namespace tpp::instance
{
	bool initialize();
	void run();
	void exit();

	inline bool init{ false };
}