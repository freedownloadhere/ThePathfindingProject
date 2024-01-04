#pragma once

#include <iostream>

#include "../../Vector3/Vector3.h"
#include "../../Utils/Utils.h"
#include "../../JNI/JNI.h"

namespace tpp::world
{
	bool initialize();

	int get_block_id(const Vector3& pos);

	inline bool init{ false };
}