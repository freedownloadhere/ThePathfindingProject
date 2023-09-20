#pragma once

#pragma once

#include <jni.h>
#include <string>
#include <Windows.h>
#include <thread>
#include <chrono>

#include "../../Vector3/Vector3.h"
#include "../../ViewAngles/ViewAngles.h"
#include "../../Utils/EnumFacing.h"
#include "../../Utils/Utils.h"

namespace tpp
{
	class Player
	{
	public:
		Player(
			JNIEnv* env,
			const jclass& mcClass,
			const jobject& mcClassInstance
		);

		bool isInit();

		void updateMainInventory();
		void updatePosition();
		void updateViewAngles();
		std::string getItem(int index);
		std::string updateAndGetItem(int index);
		Vector3 getLookingAt();
		Vector3 getBlockBelowPosition();
		Vector3 getFootPosition();
		Vector3 getHeadPosition();
		EnumFacing getFacing();
		void setViewAngles(const ViewAngles& newViewAngles);
		ViewAngles getViewAngles();
		void leftClick();
		void rightClick();
		jclass getEntityPlayerSPClass();
		jobject getMcThePlayerInstance();

	private:
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

		inline static INPUT leftClickInput[2], rightClickInput[2];

		bool m_init{ false };
		JNIEnv* env{ nullptr };
		bool init(
			JNIEnv* env,
			const jclass& mcClass,
			const jobject& mcClassInstance
		);
	};
}