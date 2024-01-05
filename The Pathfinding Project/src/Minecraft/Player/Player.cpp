#include "Player.h"

using namespace tpp;

static std::string inventory[36] = {};
static jobjectArray inv_array{ nullptr };
static INPUT lc_inputs[2], rc_inputs[2];

bool player::initialize()
{
	inv_array = reinterpret_cast<jobjectArray>(
		jni::get_obj("InventoryPlayer", "mainInventory")
	);

	lc_inputs[0].type = INPUT_MOUSE;
	lc_inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	lc_inputs[1].type = INPUT_MOUSE;
	lc_inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

	rc_inputs[0].type = INPUT_MOUSE;
	rc_inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	rc_inputs[1].type = INPUT_MOUSE;
	rc_inputs[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;

	update_pos();
	update_inv();

	init = true;
	return true;
}

void player::update_pos()
{
	position.x = jni::get_double("EntityPlayerSP", "posX");
	position.y = jni::get_double("EntityPlayerSP", "posY");
	position.z = jni::get_double("EntityPlayerSP", "posZ");
}

void player::update_below_pos()
{
	below_position = get_below_pos();
}

void player::update_viewangles()
{
	viewangles.yaw = clamp_angle(jni::get_float("EntityPlayerSP", "rotationYaw"), -180, 180);
	viewangles.pitch = jni::get_float("EntityPlayerSP", "rotationPitch");
}

void player::set_viewangles(const ViewAngles& newViewAngles)
{
	jni::call_void("EntityPlayerSP", "setRotation", newViewAngles.yaw, newViewAngles.pitch);
}

ViewAngles player::get_viewangles()
{
	update_viewangles();
	return viewangles;
}

void player::update_inv()
{
	jobject itemstack_in_slot{ nullptr };
	jstring item_name{ nullptr };

	for (int index = 0; index < 36; ++index)
	{
		itemstack_in_slot = env->GetObjectArrayElement(inv_array, index);
		if (itemstack_in_slot == nullptr)
		{
			inventory[index] = "Air";
			continue;
		}

		item_name = jni::call_str(itemstack_in_slot, "ItemStack", "getDisplayName");

		inventory[index] = env->GetStringUTFChars(item_name, 0);
	}
}

std::string player::get_item(int index)
{
	update_inv();
	return (index >= 0 && index < 36 ? inventory[index] : "Invalid index");
}

Vector3 player::get_lookingat()
{
	jobject block_pos{ nullptr };
	Vector3 pos{ 0, 0, 0 };

	block_pos = jni::call_obj("MovingObjectPosition", "getBlockPos");
	if (block_pos == nullptr)
	{
		std::cout << "Could not get the block position the player is looking at\n";
		return pos;
	}

	pos.x = jni::call_int(block_pos, "BlockPos", "getX");
	pos.y = jni::call_int(block_pos, "BlockPos", "getY");
	pos.z = jni::call_int(block_pos, "BlockPos", "getZ");

	return pos;
}

EnumFacing player::get_facing()
{
	jobject facing{ nullptr };
	int index = 0;

	facing = jni::call_obj("EntityPlayerSP", "getHorizontalFacing");
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
	update_pos();
	Vector3 result(position.x, position.y - 1, position.z);
	result.truncate2();
	return result;
}

Vector3 player::get_foot_pos()
{
	update_pos();
	return position;
}

Vector3 player::get_head_pos()
{
	update_pos();
	Vector3 result(position.x, position.y + 1, position.z);
	return result;
}

void player::left_click()
{
	SendInput(1, &lc_inputs[0], sizeof(INPUT));
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	SendInput(1, &lc_inputs[1], sizeof(INPUT));
}

void player::right_click()
{
	SendInput(1, &rc_inputs[0], sizeof(INPUT));
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	SendInput(1, &rc_inputs[1], sizeof(INPUT));
}