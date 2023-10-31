#pragma once

#include <jni.h>
#include <unordered_map>
#include <string>

#include "json.hpp"

namespace tpp
{
	inline JNIEnv* env;
}

namespace tpp::jni
{
	bool initialize();

	inline std::unordered_map<std::string, jclass> cls;
	inline std::unordered_map<std::string, jmethodID> mtd;
	inline std::unordered_map<std::string, jfieldID> fld;
	inline std::unordered_map<std::string, jobject> obj;

	bool init{ false };
}