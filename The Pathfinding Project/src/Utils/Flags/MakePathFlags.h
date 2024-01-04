#pragma once

namespace tpp::makepathflags
{
	enum MakePathFlags
	{
		NONE = 0,
		SETBLOCK = 1,
		SAFE = 2,
		USEPREVCACHE = 4,
		TRAVERSE = 8
	};
}