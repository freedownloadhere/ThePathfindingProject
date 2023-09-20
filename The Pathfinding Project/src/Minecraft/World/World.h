#pragma once

#include <iostream>
#include <jni.h>

#include "../../Vector3/Vector3.h"
#include "../../Utils/Utils.h"

namespace tpp
{
	class World
	{
	public:
		World(
			JNIEnv* env,
			const jclass& mcClass,
			const jobject& mcClassInstance
		);

		bool isInit();
		int getBlockID(const Vector3& pos);

	private:
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

		bool m_init{ false };
		JNIEnv* env{ nullptr };
		bool init(
			JNIEnv* env,
			const jclass& mcClass,
			const jobject& mcClassInstance
		);
	};
}