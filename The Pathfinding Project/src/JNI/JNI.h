#pragma once

#include <jni.h>
#include <jvmti.h>
#include <unordered_map>
#include <string>
#include <iostream>

#include "json.hpp"

namespace tpp
{
	inline JNIEnv* env;
}

namespace tpp::jni
{
	enum result
	{
		OK = 0,
		GET_VM_FAILED = 1,
		GET_ENV_FAILED = 2,
		GET_CLASS_FAILED = 3,
		GET_FIELD_FAILED = 4,
		GET_METHOD_FAILED = 5
	};

	result initialize();

	inline std::unordered_map<std::string, jclass> class_map;
	inline std::unordered_map<std::string, std::unordered_map<std::string, jmethodID>> method_map;
	inline std::unordered_map<std::string, std::unordered_map<std::string, jfieldID>> field_map;

	void debug_dump();

	jfloat get_float(const jobject& obj, const std::string& cls, const std::string& fld);
	jdouble get_double(const jobject& obj, const std::string& cls, const std::string& fld);
	jobject get_obj(const jobject& obj, const std::string& cls, const std::string& fld);
	jobject get_static_obj(const std::string& cls, const std::string& fld);

	template<typename... Ts>
	void call_void(const jobject& obj, const std::string& cls, const std::string& mtd, Ts... args)
	{
		env->CallVoidMethod(obj, jni::method_map[cls][mtd], args...);
	}

	template<typename... Ts>
	jint call_int(const jobject& obj, const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallIntMethod(obj, jni::method_map[cls][mtd], args...);
	}

	template<typename... Ts>
	jint call_static_int(const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallStaticIntMethod(jni::class_map[cls], jni::method_map[cls][mtd], args...);
	}

	template<typename... Ts>
	jobject call_obj(const jobject& obj, const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallObjectMethod(obj, jni::method_map[cls][mtd], args...);
	}

	template<typename... Ts>
	jobject new_obj(const std::string& cls, const std::string& ctor, Ts... args)
	{
		return env->NewObject(jni::class_map[cls], jni::method_map[cls][ctor], args...);
	}

	inline bool init{ false };
}