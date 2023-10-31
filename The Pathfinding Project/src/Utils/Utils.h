#pragma once

#include <iostream>
#include <jni.h>
#include <jvmti.h>
#include <string>
#include <unordered_map>
#include <algorithm>

namespace tpp
{
	jclass get_class(JNIEnv* env, const std::string& path);
	
	double clamp_angle(double angle, double min, double max);
}