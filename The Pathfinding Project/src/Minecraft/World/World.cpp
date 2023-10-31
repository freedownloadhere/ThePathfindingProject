#include "World.h"

using namespace tpp;

namespace tpp::world
{
	static jobject worldInstance{ nullptr };
}

bool tpp::world::initialize(
	const jobject& mcClassInstance
)
{
	tpp::env = env;

	tpp::world::worldInstance = jni::get_obj(mcClassInstance, "Minecraft", "theWorld");
	if (worldInstance == nullptr)
	{
		std::cout << "[-] Could not get world instance\n";
		std::cout << "[INFO] Are you currently in a world?\n";
		std::cout << "[INFO] The world object cannot be fetched if not in-game.\n";
		return false;
	}

	init = true;
	return true;
}

int tpp::world::get_block_id(const Vector3& pos)
{
	if (!tpp::world::init)
	{
		std::cout << "The world object was not initialized properly\n";
		return -1;
	}

	jobject blockPosObj{ nullptr };
	jobject blockState{ nullptr };
	jobject block{ nullptr };

	blockPosObj = jni::new_obj("BlockPos", "<init>", pos.x, pos.y, pos.z);
	if (blockPosObj == nullptr)
	{
		std::cout << "Could not create the block position object\n";
		return -1;
	}

	blockState = jni::call_obj(worldInstance, "WorldClient", "getBlockState", blockPosObj);
	if (blockState == nullptr)
	{
		std::cout << "Could not get block state at " << pos.x << " " << pos.y << " " << pos.z << "\n";
		return -1;
	}

	block = jni::call_obj(blockState, "IBlockState", "getBlock");
	if (block == nullptr)
	{
		std::cout << "Could not get the block at " << pos.x << " " << pos.y << " " << pos.z << "\n";
		return -1;
	}

	int blockID = jni::call_static_int("Block", "getIdFromBlock", block);

	tpp::env->DeleteLocalRef(blockPosObj);

	return blockID;
}