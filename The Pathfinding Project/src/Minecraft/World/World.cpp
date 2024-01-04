#include "World.h"

using namespace tpp;

bool tpp::world::initialize()
{
	init = true;
	return true;
}

int tpp::world::get_block_id(const Vector3& pos)
{
	jobject block_pos{ nullptr };
	jobject block_state{ nullptr };
	jobject block{ nullptr };

	block_pos = jni::new_obj("BlockPos", "<init>", pos.x, pos.y, pos.z);
	if (block_pos == nullptr)
	{
		std::cout << "Could not create the block position object\n";
		return -1;
	}

	block_state = jni::call_obj("WorldClient", "getBlockState", block_pos);
	if (block_state == nullptr)
	{
		std::cout << "Could not get block state at " << pos.x << " " << pos.y << " " << pos.z << "\n";
		return -1;
	}

	block = jni::call_obj(block_state, "IBlockState", "getBlock");
	if (block == nullptr)
	{
		std::cout << "Could not get the block at " << pos.x << " " << pos.y << " " << pos.z << "\n";
		return -1;
	}

	int block_id = jni::call_static_int("Block", "getIdFromBlock", block);

	tpp::env->DeleteLocalRef(block_pos);

	return block_id;
}