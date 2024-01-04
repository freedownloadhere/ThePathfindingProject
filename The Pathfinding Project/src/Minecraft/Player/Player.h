#pragma once

#pragma once

#include <string>
#include <Windows.h>
#include <thread>
#include <chrono>

#include "../../Vector3/Vector3.h"
#include "../../ViewAngles/ViewAngles.h"
#include "../../Utils/EnumFacing.h"
#include "../../Utils/Utils.h"
#include "../../JNI/JNI.h"

namespace tpp::player
{
	bool initialize();

	void update_inv();
	void update_pos();
	void update_viewangles();
	std::string get_item(int index);
	Vector3 get_lookingat();
	Vector3 get_below_pos();
	Vector3 get_foot_pos();
	Vector3 get_head_pos();
	EnumFacing get_facing();
	void set_viewangles(const ViewAngles& newViewAngles);
	ViewAngles get_viewangles();
	void left_click();
	void right_click();

	inline bool init{ false };
}