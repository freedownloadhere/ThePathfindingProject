#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <map>
#include <set>
#include <vector>
#include <random>
#include <thread>

#include "../Utils/Timer/Timer.h"
#include "../Utils/Block/Block.h"
#include "../Minecraft/Minecraft.h"
#include "../Vector3/AstarVector3/AstarVector3.h"
#include "../Utils/Flags/MakePathFlags.h"

namespace tpp::pathfinder
{
	struct
	{
		Vector3 start{ 0, 0, 0 };
		Vector3 target{ 0, 0, 0 };
		int flags{ 0 };
		std::string block_to_set{ "stone" };
	}
	inline state;

	bool initialize();

	bool go_to(Vector3 target, int flags = 0, const std::string& blockToSet = "none");
	bool make_path(Vector3 start, Vector3 target, int flags = 0, const std::string& blockToSet = "none");

	std::list<Vector3> try_straight_path(const Vector3& start, const Vector3& target);
	std::list<Vector3> default_astar(const Vector3& start, const Vector3& target);

	std::vector<std::pair<Vector3, int>> make_navmap(const std::list<Vector3>& path);
	bool list_contains(const AstarVector3& element, const std::vector<AstarVector3>& heap);
	bool is_walkable(const AstarVector3& coordinates);

	inline bool init{ false };
}