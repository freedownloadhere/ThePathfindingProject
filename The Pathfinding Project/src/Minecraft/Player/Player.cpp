#include "Player.h"

using namespace tpp;

namespace tpp::player
{
	static Vector3 position{ 0, 0, 0 };
	static ViewAngles viewAngles{ 0, 0 };
	static std::string inventory[36] = {};

	static jobject
		mcClassInstance{ nullptr },
		mcThePlayerInstance{ nullptr },
		inventoryInstance{ nullptr };
	static jobjectArray
		mainInventoryArray{ nullptr };
	static INPUT
		leftClickInput[2],
		rightClickInput[2];
}

bool player::initialize(
	const jobject& mcClassInstance
)
{
	player::mcClassInstance = mcClassInstance;

	mcThePlayerInstance = jni::get_obj(mcClassInstance, "Minecraft", "thePlayer");
	if (mcThePlayerInstance == nullptr)
	{
		std::cout << "[-] Failed to get object field thePlayer\n";
		std::cout << "[INFO] Are you currently in a world?\n";
		std::cout << "[INFO] The player object cannot be fetched if not in-game.\n";
		return false;
	}

	inventoryInstance = jni::get_obj(mcThePlayerInstance, "EntityPlayerSP", "inventory");
	if (inventoryInstance == nullptr)
	{
		std::cout << "[-] Failed to get object field inventory\n";
		return false;
	}

	mainInventoryArray = reinterpret_cast<jobjectArray>(
		jni::get_obj(inventoryInstance, "InventoryPlayer", "mainInventory")
	);

	leftClickInput[0].type = INPUT_MOUSE;
	leftClickInput[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	leftClickInput[1].type = INPUT_MOUSE;
	leftClickInput[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

	rightClickInput[0].type = INPUT_MOUSE;
	rightClickInput[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	rightClickInput[1].type = INPUT_MOUSE;
	rightClickInput[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;

	update_pos();
	update_inv();

	init = true;
	return true;
}

void player::update_pos()
{
	position.x = jni::get_double(mcThePlayerInstance, "EntityPlayerSP", "posX");
	position.y = jni::get_double(mcThePlayerInstance, "EntityPlayerSP", "posY");
	position.z = jni::get_double(mcThePlayerInstance, "EntityPlayerSP", "posZ");
}

void player::update_viewangles()
{
	viewAngles.yaw = clamp_angle(jni::get_float(mcThePlayerInstance, "EntityPlayerSP", "rotationYaw"), -180, 180);
	viewAngles.pitch = jni::get_float(mcThePlayerInstance, "EntityPlayerSP", "rotationPitch");
}

void player::set_viewangles(const ViewAngles& newViewAngles)
{
	jni::call_void(mcThePlayerInstance, "Entity", "setRotation", newViewAngles.yaw, newViewAngles.pitch);
}

ViewAngles player::get_viewangles()
{
	update_viewangles();
	return viewAngles;
}

void player::update_inv()
{
	jobject itemStackInSlot{ nullptr };
	jstring itemName{ nullptr };

	for (int index = 0; index < 36; ++index)
	{
		itemStackInSlot = env->GetObjectArrayElement(mainInventoryArray, index);
		if (itemStackInSlot == nullptr)
		{
			inventory[index] = "Air";
			continue;
		}

		itemName = static_cast<jstring>(
			jni::call_obj(itemStackInSlot, "ItemStack", "getDisplayName")
		);

		inventory[index] = env->GetStringUTFChars(itemName, 0);
	}
}

std::string player::get_item(int index)
{
	update_inv();

	return (index >= 0 && index < 36 ? inventory[index] : "Invalid index");
}

Vector3 player::get_lookingat()
{
	jobject mouseOverInstance{ nullptr };
	jobject blockPos{ nullptr };
	Vector3 pos{ 0, 0, 0 };

	mouseOverInstance = jni::get_obj(mcClassInstance, "Minecraft", "objectMouseOver");
	if (mouseOverInstance == nullptr)
	{
		std::cout << "Could not get the objMouseOver instance\n";
		return pos;
	}

	blockPos = jni::call_obj(mouseOverInstance, "MovingObjectPosition", "getBlockPos");
	if (blockPos == nullptr)
	{
		std::cout << "Could not get the block position the player is looking at\n";
		return pos;
	}

	pos.x = jni::call_int(blockPos, "BlockPos", "getX");
	pos.y = jni::call_int(blockPos, "BlockPos", "getY");
	pos.z = jni::call_int(blockPos, "BlockPos", "getZ");

	return pos;
}

EnumFacing player::get_facing()
{
	jobject facing{ nullptr };
	int index = 0;

	facing = jni::call_obj(mcThePlayerInstance, "EntityPlayerSP", "getHorizontalFacing");
	if (facing == nullptr)
	{
		std::cout << "Could not get the direction the player is facing\n";
		return EnumFacing::DOWN;
	}

	index = jni::call_int(facing, "EnumFacing", "getIndex");
	if (index == 0) std::cout << "Invalid horizontal enumfacing index\n";

	return (EnumFacing)index;
}

Vector3 player::get_below_pos()
{
	player::update_pos();
	Vector3 result(position.x, position.y - 1, position.z);
	result.truncate2();
	return result;
}

Vector3 player::get_foot_pos()
{
	player::update_pos();
	return player::position;
}

Vector3 player::get_head_pos()
{
	update_pos();
	Vector3 result(position.x, position.y + 1, position.z);
	return result;
}

void player::left_click()
{
	SendInput(1, &leftClickInput[0], sizeof(INPUT));
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	SendInput(1, &leftClickInput[1], sizeof(INPUT));
}

void player::right_click()
{
	SendInput(1, &rightClickInput[0], sizeof(INPUT));
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	SendInput(1, &rightClickInput[1], sizeof(INPUT));
}