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
	bool initialize(
		const jclass& mcClass,
		const jobject& mcClassInstance
	);

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

	Vector3 position{ 0, 0, 0 };
	ViewAngles viewAngles{ 0, 0 };
	std::string inventory[36] = {};

	jclass
		mcClass{ nullptr },
		EntityPlayerSPClass{ nullptr },
		InventoryPlayerClass{ nullptr },
		itemStackClass{ nullptr },
		enumFacingClass{ nullptr };
	jobject
		mcClassInstance{ nullptr },
		mcThePlayerInstance{ nullptr },
		inventoryInstance{ nullptr };
	jobjectArray
		mainInventoryArray{ nullptr };
	jmethodID
		getBlockPos{ nullptr },
		blockPosX{ nullptr },
		blockPosY{ nullptr },
		blockPosZ{ nullptr },
		setRotation{ nullptr },
		getHorizontalFacing{ nullptr },
		getEnumFacingIndex{ nullptr },
		displayNameGetter{ nullptr };
	jfieldID
		objectMouseOver{ nullptr },
		positionX{ nullptr },
		positionY{ nullptr },
		positionZ{ nullptr },
		yawField{ nullptr },
		pitchField{ nullptr };
	INPUT 
		leftClickInput[2], 
		rightClickInput[2];

	bool init{ false };
}