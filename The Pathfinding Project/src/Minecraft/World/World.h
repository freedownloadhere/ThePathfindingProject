#pragma once

#include <iostream>

#include "../../Vector3/Vector3.h"
#include "../../Utils/Utils.h"
#include "../../JNI/JNI.h"

namespace tpp::world
{
	bool initialize(
		const jclass& mcClass,
		const jobject& mcClassInstance
	);

	int get_block_id(const Vector3& pos);

	jclass
		worldClientClass{ nullptr },
		blockPosClass{ nullptr },
		blockClass{ nullptr };
	jmethodID
		getBlockState{ nullptr },
		getBlock{ nullptr },
		blockPosConstructor{ nullptr },
		getIDfromBlock{ nullptr };
	jobject
		worldInstance{ nullptr };

	bool init{ false };
}