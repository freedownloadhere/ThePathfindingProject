#include "Player.h"

using namespace tpp;

bool tpp::player::initialize(
	const jclass& mcClass,
	const jobject& mcClassInstance
)
{
	tpp::player::mcClass = mcClass;
	tpp::player::mcClassInstance = mcClassInstance;

	tpp::player::EntityPlayerSPClass = tpp::getClass(tpp::env, "net/minecraft/client/entity/EntityPlayerSP");
	if (tpp::player::EntityPlayerSPClass == nullptr)
	{
		std::cout << "[-] Failed to get class EntityPlayerSP\n";
		return false;
	}

	jfieldID playerFieldID{ tpp::env->GetFieldID(mcClass, "field_71439_g", "Lnet/minecraft/client/entity/EntityPlayerSP;") };
	if (playerFieldID == nullptr)
	{
		std::cout << "[-] Failed to get object field ID thePlayer\n";
		return false;
	}

	tpp::player::mcThePlayerInstance = tpp::env->GetObjectField(mcClassInstance, playerFieldID);
	if (tpp::player::mcThePlayerInstance == nullptr)
	{
		std::cout << "[-] Failed to get object field thePlayer\n";
		std::cout << "[INFO] Are you currently in a world?\n";
		std::cout << "[INFO] The player object cannot be fetched if not in-game.\n";
		return false;
	}

	tpp::player::InventoryPlayerClass = tpp::getClass(tpp::env, "net/minecraft/entity/player/InventoryPlayer");
	if (tpp::player::InventoryPlayerClass == nullptr)
	{
		std::cout << "[-] Failed to get class InventoryPlayer\n";
		return false;
	}

	jfieldID inventoryID{ tpp::env->GetFieldID(EntityPlayerSPClass, "field_71071_by", "Lnet/minecraft/entity/player/InventoryPlayer;") };
	if (inventoryID == nullptr)
	{
		std::cout << "[-] Failed to get inventory field ID!\n";
		return false;
	}

	tpp::player::inventoryInstance = tpp::env->GetObjectField(mcThePlayerInstance, inventoryID);
	if (inventoryInstance == nullptr)
	{
		std::cout << "[-] Failed to get object field inventory\n";
		return false;
	}

	jfieldID mainInventoryFieldID = tpp::env->GetFieldID(tpp::player::InventoryPlayerClass, "field_70462_a", "[Lnet/minecraft/item/ItemStack;");
	if (mainInventoryFieldID == nullptr)
	{
		std::cout << "[-] Could not get main inventory field ID\n";
		return false;
	}

	jobject mainInventoryObj = tpp::env->GetObjectField(tpp::player::inventoryInstance, mainInventoryFieldID);
	if (mainInventoryObj == nullptr)
	{
		std::cout << "[-] Could not get main inventory array object\n";
		return false;
	}

	tpp::player::mainInventoryArray = static_cast<jobjectArray>(mainInventoryObj);

	tpp::player::itemStackClass = tpp::getClass(tpp::env, "net/minecraft/item/ItemStack");
	if (tpp::player::itemStackClass == nullptr)
	{
		std::cout << "[-] Could not find class itemStack\n";
		return false;
	}

	tpp::player::displayNameGetter = tpp::env->GetMethodID(tpp::player::itemStackClass, "func_82833_r", "()Ljava/lang/String;");
	if (tpp::player::displayNameGetter == nullptr)
	{
		std::cout << "[-] Could not get the display name getter method\n";
		return false;
	}

	tpp::player::positionX = tpp::env->GetFieldID(tpp::player::EntityPlayerSPClass, "field_70165_t", "D");
	if (tpp::player::positionX == nullptr)
	{
		std::cout << "[-] Could not get player X position field ID\n";
		return false;
	}

	tpp::player::positionY = tpp::env->GetFieldID(tpp::player::EntityPlayerSPClass, "field_70163_u", "D");
	if (tpp::player::positionY == nullptr)
	{
		std::cout << "[-] Could not get player Y position field ID\n";
		return false;
	}

	tpp::player::positionZ = tpp::env->GetFieldID(tpp::player::EntityPlayerSPClass, "field_70161_v", "D");
	if (tpp::player::positionZ == nullptr)
	{
		std::cout << "[-] Could not get player Z position field ID\n";
		return false;
	}

	tpp::player::yawField = tpp::env->GetFieldID(tpp::player::EntityPlayerSPClass, "field_70177_z", "F");
	if (tpp::player::yawField == nullptr)
	{
		std::cout << "[-] Could not get player yaw field\n";
		return false;
	}

	tpp::player::pitchField = tpp::env->GetFieldID(tpp::player::EntityPlayerSPClass, "field_70125_A", "F");
	if (tpp::player::pitchField == nullptr)
	{
		std::cout << "[-] Could not get player yaw field\n";
		return false;
	}

	jclass movingObjectPositionClass = tpp::getClass(tpp::env, "net/minecraft/util/MovingObjectPosition");
	if (movingObjectPositionClass == nullptr)
	{
		std::cout << "[-] Could not get the objectMouseOver class\n";
		return false;
	}

	tpp::player::objectMouseOver = tpp::env->GetFieldID(tpp::player::mcClass, "field_71476_x", "Lnet/minecraft/util/MovingObjectPosition;");
	if (objectMouseOver == nullptr)
	{
		std::cout << "[-] Could not get the objectMouseOver minecraft class field\n";
		return false;
	}

	jclass blockPosClass = tpp::getClass(tpp::env, "net/minecraft/util/BlockPos");
	if (blockPosClass == nullptr)
	{
		std::cout << "[-] Could not get block pos class\n";
		return false;
	}

	tpp::player::getBlockPos = tpp::env->GetMethodID(movingObjectPositionClass, "func_178782_a", "()Lnet/minecraft/util/BlockPos;");
	if (tpp::player::getBlockPos == nullptr)
	{
		std::cout << "[-] Could not get the getBlockPos method\n";
		return false;
	}

	tpp::player::blockPosX = tpp::env->GetMethodID(blockPosClass, "func_177958_n", "()I");
	if (tpp::player::blockPosX == nullptr)
	{
		std::cout << "[-] Could not get the getX block pos method\n";
		return false;
	}

	tpp::player::blockPosY = tpp::env->GetMethodID(blockPosClass, "func_177956_o", "()I");
	if (tpp::player::blockPosY == nullptr)
	{
		std::cout << "[-] Could not get the getY block pos method\n";
		return false;
	}

	tpp::player::blockPosZ = tpp::env->GetMethodID(blockPosClass, "func_177952_p", "()I");
	if (tpp::player::blockPosZ == nullptr)
	{
		std::cout << "[-] Could not get the getZ block pos method\n";
		return false;
	}

	jclass entityClass = tpp::getClass(tpp::env, "net/minecraft/entity/Entity");
	if (entityClass == nullptr)
	{
		std::cout << "[-] Could not get the Entity class\n";
		return false;
	}

	tpp::player::setRotation = tpp::env->GetMethodID(entityClass, "func_70101_b", "(FF)V");
	if (tpp::player::setRotation == nullptr)
	{
		std::cout << "[-] Could not get the setRotation method\n";
		return false;
	}

	tpp::player::enumFacingClass = tpp::getClass(tpp::env, "net/minecraft/util/EnumFacing");
	if (tpp::player::enumFacingClass == nullptr)
	{
		std::cout << "[-] Could not get the enumFacing class\n";
		return false;
	}

	tpp::player::getHorizontalFacing = tpp::env->GetMethodID(tpp::player::EntityPlayerSPClass, "func_174811_aO", "()Lnet/minecraft/util/EnumFacing;");
	if (tpp::player::getHorizontalFacing == nullptr)
	{
		std::cout << "[-] Could not get the getHorizontalFacing method\n";
		return false;
	}

	tpp::player::getEnumFacingIndex = tpp::env->GetMethodID(tpp::player::enumFacingClass, "func_176745_a", "()I");
	if (tpp::player::getEnumFacingIndex == nullptr)
	{
		std::cout << "[-] Could not get the enumfacing getIndex method\n";
		return false;
	}

	tpp::player::leftClickInput[0].type = INPUT_MOUSE;
	tpp::player::leftClickInput[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	tpp::player::leftClickInput[1].type = INPUT_MOUSE;
	tpp::player::leftClickInput[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

	tpp::player::rightClickInput[0].type = INPUT_MOUSE;
	tpp::player::rightClickInput[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	tpp::player::rightClickInput[1].type = INPUT_MOUSE;
	tpp::player::rightClickInput[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;

	tpp::player::update_pos();
	tpp::player::update_inv();

	return true;
}

void tpp::player::update_pos()
{
	tpp::player::position.x = (double)tpp::env->GetDoubleField(tpp::player::mcThePlayerInstance, tpp::player::positionX);
	tpp::player::position.y = (double)tpp::env->GetDoubleField(tpp::player::mcThePlayerInstance, tpp::player::positionY);
	tpp::player::position.z = (double)tpp::env->GetDoubleField(tpp::player::mcThePlayerInstance, tpp::player::positionZ);
}

void tpp::player::update_viewangles()
{
	tpp::player::viewAngles.yaw = tpp::env->GetFloatField(tpp::player::mcThePlayerInstance, tpp::player::yawField);
	tpp::player::viewAngles.pitch = tpp::env->GetFloatField(tpp::player::mcThePlayerInstance, tpp::player::pitchField);
	tpp::player::viewAngles.yaw = tpp::clampAngle(tpp::player::viewAngles.yaw, -180, 180);
}

void tpp::player::set_viewangles(const ViewAngles& newViewAngles)
{
	tpp::env->CallVoidMethod(tpp::player::mcThePlayerInstance, tpp::player::setRotation, newViewAngles.yaw, newViewAngles.pitch);
}

ViewAngles tpp::player::get_viewangles()
{
	tpp::player::update_viewangles();
	return tpp::player::viewAngles;
}

void tpp::player::update_inv()
{
	jobject itemStackInSlot{ nullptr };
	jstring itemName{ nullptr };

	for (int index = 0; index < 36; ++index)
	{
		itemStackInSlot = tpp::env->GetObjectArrayElement(tpp::player::mainInventoryArray, index);
		if (itemStackInSlot == nullptr)
		{
			tpp::player::inventory[index] = "Air";
			continue;
		}

		itemName = static_cast<jstring>(tpp::env->CallObjectMethod(itemStackInSlot, displayNameGetter));

		tpp::player::inventory[index] = tpp::env->GetStringUTFChars(itemName, 0);
	}
}

std::string tpp::player::get_item(int index)
{
	tpp::player::update_inv();

	if (index >= 0 and index < 36) return tpp::player::inventory[index];
	else return "";
}

Vector3 tpp::player::get_lookingat()
{
	jobject mouseOverInstance{ nullptr };
	jobject blockPos{ nullptr };
	Vector3 pos{ 0, 0, 0 };

	mouseOverInstance = tpp::env->GetObjectField(tpp::player::mcClassInstance, tpp::player::objectMouseOver);
	if (mouseOverInstance == nullptr)
	{
		std::cout << "Could not get the objMouseOver instance\n";
		return pos;
	}

	blockPos = tpp::env->CallObjectMethod(mouseOverInstance, tpp::player::getBlockPos);
	if (blockPos == nullptr)
	{
		std::cout << "Could not get the block position the player is looking at\n";
		return pos;
	}

	pos.x = tpp::env->CallIntMethod(blockPos, tpp::player::blockPosX);
	pos.y = tpp::env->CallIntMethod(blockPos, tpp::player::blockPosY);
	pos.z = tpp::env->CallIntMethod(blockPos, tpp::player::blockPosZ);

	return pos;
}

EnumFacing tpp::player::get_facing()
{
	jobject facing{ nullptr };
	int index = 0;

	facing = tpp::env->CallObjectMethod(tpp::player::mcThePlayerInstance, tpp::player::getHorizontalFacing);
	if (facing == nullptr)
	{
		std::cout << "Could not get the direction the player is facing\n";
		return EnumFacing::DOWN;
	}

	index = tpp::env->CallIntMethod(facing, tpp::player::getEnumFacingIndex);
	if (index == 0)
		std::cout << "Invalid horizontal enumfacing index\n";

	return (EnumFacing)index;
}

Vector3 tpp::player::get_below_pos()
{
	tpp::player::update_pos();
	Vector3 result(tpp::player::position.x, tpp::player::position.y - 1, tpp::player::position.z);
	result.truncate2();
	return result;
}

Vector3 tpp::player::get_foot_pos()
{
	tpp::player::update_pos();
	return tpp::player::position;
}

Vector3 tpp::player::get_head_pos()
{
	tpp::player::update_pos();
	Vector3 result(tpp::player::position.x, tpp::player::position.y + 1, tpp::player::position.z);
	return result;
}

void tpp::player::left_click()
{
	SendInput(1, &tpp::player::leftClickInput[0], sizeof(INPUT));
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	SendInput(1, &tpp::player::leftClickInput[1], sizeof(INPUT));
}

void tpp::player::right_click()
{
	SendInput(1, &tpp::player::rightClickInput[0], sizeof(INPUT));
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	SendInput(1, &tpp::player::rightClickInput[1], sizeof(INPUT));
}