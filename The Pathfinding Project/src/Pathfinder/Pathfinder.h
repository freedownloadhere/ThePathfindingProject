#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <map>
#include <set>
#include <random>
#include <thread>

#include "../Utils/Timer/Timer.h"
#include "../Utils/Block/Block.h"
#include "../Minecraft/Minecraft.h"
#include "../Vector3/AstarVector3/AstarVector3.h"

namespace tpp
{
	class Pathfinder
	{
	public:
		Pathfinder(const std::shared_ptr<Minecraft>& minecraft);

		bool goTo(const Vector3& target, const std::string& blockToSet = "none");
		bool makePath(const Vector3& start, const Vector3& target, const std::string& blockToSet = "none");

		bool isInit();

	private:
		Vector3 directionalVector[12] =
		{
			{0, 0, -1}, // NORTH
			{1, 0, 0},  // EAST
			{0, 0, 1},  // SOUTH
			{-1, 0, 0}, // WEST
			{0, -1, -1}, // NORTH + DOWN
			{1, -1, 0},  // EAST + DOWN
			{0, -1, 1},  // SOUTH + DOWN
			{-1, -1, 0}, // WEST + DOWN
			{0, 1, -1}, // NORTH + UP
			{1, 1, 0},  // EAST + UP
			{0, 1, 1},  // SOUTH + UP
			{-1, 1, 0}, // WEST + UP
		};
		const Vector3 upOne{ 0, 1, 0 };
		const Vector3 upTwo{ 0, 2, 0 };
		const Vector3 getBlockGoalpoint{ 0.5, 1, 0.5 };
		const double errorMargin{ 0.3 };

		/// <summary>
		/// Lines are, in order:
		/// W, A, S and D.
		/// Col 0 is press, col 1 is release.
		/// </summary>
		INPUT wasd[4][2];

		std::shared_ptr<Minecraft> minecraft{ nullptr };

		std::mt19937 shuffler;
		std::map<Vector3, bool> walkableBlockCache;

		std::list<Vector3> defaultAstar(const Vector3& start, const Vector3& target);
		void traversePath(const std::list<Vector3>& path);

		std::vector<std::pair<Vector3, int>> makeWalkMap(const std::list<Vector3>& path);
		bool listContains(const AstarVector3& element, const std::vector<AstarVector3>& heap);
		bool isWalkable(const AstarVector3& coordinates);

		bool mInit{ false };
		bool init(const std::shared_ptr<Minecraft>& minecraft);
	};
}