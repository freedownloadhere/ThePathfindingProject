#pragma once

#include <unordered_set>
#include <memory>

#include "../../Vector3/Vector3.h"

namespace tpp
{
	struct Block
	{
		int id;
		Vector3 pos;

		inline static std::unordered_set<int>
			blocksToBreak = { 1, 14, 15, 16, 21, 56, 73, 74, 129 },
			blocksToOpen = { 54, 146 },
			nonSolid = {
				0, 6, 8, 9, 10, 11, 27, 28,
				30, 31, 32, 37, 38, 39, 40,
				50, 51, 55, 59, 63, 65, 66,
				68, 69, 70, 72, 75, 76, 77,
				83, 90, 104, 105, 106, 115,
				131, 132, 141, 142, 143, 147,
				148, 157, 175, 176, 177, 207
			};

		bool toBreak() const;
		bool toOpen() const;
		bool isAir() const;
		bool isWalkable() const;
	};
}