#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <map>
#include <set>
#include <vector>
#include <random>
#include <thread>
#include <memory>
#include <mutex>

#include "../Utils/Timer/Timer.h"
#include "../Utils/Block/Block.h"
#include "../Minecraft/Minecraft.h"
#include "../Vector3/AstarVector3/AstarVector3.h"
#include "../Utils/Flags/MakePathFlags.h"
#include "../GUI/GUI.h"

namespace tpp::pathfinder
{
	constexpr int PLAYER_POS = -300000001;

	struct state_struct
	{
		Vector3 start{ 0, 0, 0 };
		Vector3 target{ 0, 0, 0 };
		int flags{ 0 };
		std::string block_to_set{ "stone" };

		bool should_run{ false };
	};

	inline std::unique_ptr<state_struct> state;

	bool initialize();
	bool make_path();
	void update_state();

	std::list<Vector3> try_straight_path();
	std::list<Vector3> default_astar();

	std::vector<std::pair<Vector3, int>> make_navmap(const std::list<Vector3>& path);
	bool list_contains(const AstarVector3& element, const std::vector<AstarVector3>& heap);
	bool is_walkable(const AstarVector3& coordinates);

	inline bool init{ false };
}