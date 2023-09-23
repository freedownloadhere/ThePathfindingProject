#pragma once

namespace tpp
{
	enum class MakePathFlags
	{
		NONE = 0,
		SETBLOCK = 1,
		SAFE = 2,
		USEPREVCACHE = 4
	};
}