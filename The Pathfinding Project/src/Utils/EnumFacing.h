#pragma once

namespace tpp
{
	enum class EnumFacing
	{
		DOWN = 0,  // -y (0, -1, 0)
		UP = 1,	   // +y (0, 1, 0)
		NORTH = 2, // -z (0, 0, -1)
		SOUTH = 3, // +z (0, 0, 1)
		WEST = 4,  // -x (-1, 0, 0)
		EAST = 5   // +x (1, 0, 0)
	};
}