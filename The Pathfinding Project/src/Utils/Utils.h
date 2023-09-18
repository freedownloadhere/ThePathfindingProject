#pragma once

#include <iostream>
#include <jni.h>
#include <jvmti.h>
#include <string>
#include <unordered_map>
#include <algorithm>

namespace tpp
{
	jclass getClass(JNIEnv* env, const std::string& path);

	double clampAngle(double angle, double min, double max);
}