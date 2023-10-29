#include "World.h"

using namespace tpp;

bool tpp::world::initialize(
	const jclass& mcClass,
	const jobject& mcClassInstance
)
{
	tpp::env = env;

	tpp::world::worldClientClass = tpp::getClass(tpp::env, "net/minecraft/client/multiplayer/tpp::worldClient");
	if (worldClientClass == nullptr)
	{
		std::cout << "[-] Could not find the world client class\n";
		return false;
	}

	tpp::world::blockPosClass = tpp::getClass(tpp::env, "net/minecraft/util/BlockPos");
	if (blockPosClass == nullptr)
	{
		std::cout << "[-] Could not find the block pos class\n";
		return false;
	}

	tpp::world::blockClass = tpp::getClass(tpp::env, "net/minecraft/block/Block");
	if (blockClass == nullptr)
	{
		std::cout << "[-] Could not find the block class\n";
		return false;
	}

	jfieldID worldFieldID = tpp::env->GetFieldID(mcClass, "field_71441_e", "Lnet/minecraft/client/multiplayer/tpp::worldClient;");
	if (worldFieldID == nullptr)
	{
		std::cout << "[-] Could not get thetpp::world field ID\n";
		return false;
	}

	tpp::world::worldInstance = tpp::env->GetObjectField(mcClassInstance, worldFieldID);
	if (worldInstance == nullptr)
	{
		std::cout << "[-] Could not get world instance\n";
		std::cout << "[INFO] Are you currently in a world?\n";
		std::cout << "[INFO] The world object cannot be fetched if not in-game.\n";
		return false;
	}

	tpp::world::getBlockState = tpp::env->GetMethodID(tpp::world::worldClientClass, "func_180495_p", "(Lnet/minecraft/util/BlockPos;)Lnet/minecraft/block/state/IBlockState;");
	if (tpp::world::getBlockState == nullptr)
	{
		std::cout << "[-] Could not get block state method\n";
		return false;
	}

	tpp::world::blockPosConstructor = tpp::env->GetMethodID(tpp::world::blockPosClass, "<init>", "(DDD)V");
	if (tpp::world::blockPosConstructor == nullptr)
	{
		std::cout << "[-] Could not get block pos constructor ID\n";
		return false;
	}

	jclass blockStateInterface = tpp::getClass(tpp::env, "net/minecraft/block/state/IBlockState");
	if (blockStateInterface == nullptr)
	{
		std::cout << "[-] Could not get the block state interface\n";
		return false;
	}

	tpp::world::getBlock = tpp::env->GetMethodID(blockStateInterface, "func_177230_c", "()Lnet/minecraft/block/Block;");
	if (tpp::world::getBlock == nullptr)
	{
		std::cout << "[-] Could not get block method\n";
		return false;
	}

	tpp::world::getIDfromBlock = tpp::env->GetStaticMethodID(tpp::world::blockClass, "func_149682_b", "(Lnet/minecraft/block/Block;)I");
	if (tpp::world::getIDfromBlock == nullptr)
	{
		std::cout << "[-] Could not get the getIDfromBlock static method\n";
		return false;
	}

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

	blockPosObj = tpp::env->NewObject(tpp::world::blockPosClass, tpp::world::blockPosConstructor, pos.x, pos.y, pos.z);
	if (blockPosObj == nullptr)
	{
		std::cout << "Could not create the block position object\n";
		return -1;
	}

	blockState = tpp::env->CallObjectMethod(tpp::world::worldInstance, getBlockState, blockPosObj);
	if (blockState == nullptr)
	{
		std::cout << "Could not get block state at " << pos.x << " " << pos.y << " " << pos.z << "\n";
		return -1;
	}

	block = tpp::env->CallObjectMethod(blockState, tpp::world::getBlock);
	if (block == nullptr)
	{
		std::cout << "Could not get the block at " << pos.x << " " << pos.y << " " << pos.z << "\n";
		return -1;
	}

	int blockID = tpp::env->CallStaticIntMethod(blockClass, getIDfromBlock, block);

	tpp::env->DeleteLocalRef(blockPosObj);

	return blockID;
}