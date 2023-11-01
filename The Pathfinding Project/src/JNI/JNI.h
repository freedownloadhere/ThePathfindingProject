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

	inline std::unordered_map<std::string, jclass> class_map;
	inline std::unordered_map<std::string, std::unordered_map<std::string, jmethodID>> method_map;
	inline std::unordered_map<std::string, std::unordered_map<std::string, jfieldID>> field_map;
	inline std::unordered_map<std::string, jobject> object_map;

	result initialize();

	void set_instance(const std::string& cls, const jobject& obj);

	jobject get_obj(const jobject& obj, const std::string& cls, const std::string& fld);
	jboolean get_bool(const jobject& obj, const std::string& cls, const std::string& fld);
	jbyte get_byte(const jobject& obj, const std::string& cls, const std::string& fld);
	jchar get_char(const jobject& obj, const std::string& cls, const std::string& fld);
	jshort get_short(const jobject& obj, const std::string& cls, const std::string& fld);
	jint get_int(const jobject& obj, const std::string& cls, const std::string& fld);
	jlong get_long(const jobject& obj, const std::string& cls, const std::string& fld);
	jfloat get_float(const jobject& obj, const std::string& cls, const std::string& fld);
	jdouble get_double(const jobject& obj, const std::string& cls, const std::string& fld);
	jobject get_obj(const std::string& cls, const std::string& fld);
	jboolean get_bool(const std::string& cls, const std::string& fld);
	jbyte get_byte(const std::string& cls, const std::string& fld);
	jchar get_char(const std::string& cls, const std::string& fld);
	jshort get_short(const std::string& cls, const std::string& fld);
	jint get_int(const std::string& cls, const std::string& fld);
	jlong get_long(const std::string& cls, const std::string& fld);
	jfloat get_float(const std::string& cls, const std::string& fld);
	jdouble get_double(const std::string& cls, const std::string& fld);

	jobject get_static_obj(const std::string& cls, const std::string& fld);
	jboolean get_static_bool(const std::string& cls, const std::string& fld);
	jbyte get_static_byte(const std::string& cls, const std::string& fld);
	jchar get_static_char(const std::string& cls, const std::string& fld);
	jshort get_static_short(const std::string& cls, const std::string& fld);
	jint get_static_int(const std::string& cls, const std::string& fld);
	jlong get_static_long(const std::string& cls, const std::string& fld);
	jfloat get_static_float(const std::string& cls, const std::string& fld);
	jdouble get_static_double(const std::string& cls, const std::string& fld);
	
	template<typename... Ts>
	void call_void(const jobject& obj, const std::string& cls, const std::string& mtd, Ts... args)
	{
		env->CallVoidMethod(obj, jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jobject call_obj(const jobject& obj, const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallObjectMethod(obj, jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jstring call_str(const jobject& obj, const std::string& cls, const std::string& mtd, Ts... args)
	{
		return (jstring)env->CallObjectMethod(obj, jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jboolean call_bool(const jobject& obj, const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallBooleanMethod(obj, jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jbyte call_byte(const jobject& obj, const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallByteMethod(obj, jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jchar call_char(const jobject& obj, const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallCharMethod(obj, jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jshort call_short(const jobject& obj, const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallShortMethod(obj, jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jint call_int(const jobject& obj, const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallIntMethod(obj, jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jlong call_long(const jobject& obj, const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallLongMethod(obj, jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jfloat call_float(const jobject& obj, const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallFloatMethod(obj, jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jdouble call_double(const jobject& obj, const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallDoubleMethod(obj, jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	void call_void(const std::string& cls, const std::string& mtd, Ts... args)
	{
		env->CallVoidMethod(jni::object_map[cls], jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jobject call_obj(const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallObjectMethod(jni::object_map[cls], jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jstring call_str(const std::string& cls, const std::string& mtd, Ts... args)
	{
		return (jstring)env->CallObjectMethod(jni::object_map[cls], jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jboolean call_bool(const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallBooleanMethod(jni::object_map[cls], jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jbyte call_byte(const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallByteMethod(jni::object_map[cls], jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jchar call_char(const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallCharMethod(jni::object_map[cls], jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jshort call_short(const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallShortMethod(jni::object_map[cls], jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jint call_int(const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallIntMethod(jni::object_map[cls], jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jlong call_long(const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallLongMethod(jni::object_map[cls], jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jfloat call_float(const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallFloatMethod(jni::object_map[cls], jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jdouble call_double(const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallDoubleMethod(jni::object_map[cls], jni::method_map[cls][mtd], args...);
	}

	template<typename... Ts>
	void call_static_void(const std::string& cls, const std::string& mtd, Ts... args)
	{
		env->CallStaticVoidMethod(jni::class_map[cls], jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jobject call_static_obj(const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallStaticObjectMethod(jni::class_map[cls], jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jboolean call_static_bool(const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallStaticBooleanMethod(jni::class_map[cls], jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jbyte call_static_byte(const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallStaticByteMethod(jni::class_map[cls], jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jchar call_static_char(const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallStaticCharMethod(jni::class_map[cls], jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jshort call_static_short(const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallStaticShortMethod(jni::class_map[cls], jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jint call_static_int(const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallStaticIntMethod(jni::class_map[cls], jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jlong call_static_long(const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallStaticLongMethod(jni::class_map[cls], jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jfloat call_static_float(const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallStaticFloatMethod(jni::class_map[cls], jni::method_map[cls][mtd], args...);
	}
	template<typename... Ts>
	jdouble call_static_double(const std::string& cls, const std::string& mtd, Ts... args)
	{
		return env->CallStaticDoubleMethod(jni::class_map[cls], jni::method_map[cls][mtd], args...);
	}

	template<typename... Ts>
	jobject new_obj(const std::string& cls, const std::string& ctor, Ts... args)
	{
		return env->NewObject(jni::class_map[cls], jni::method_map[cls][ctor], args...);
	}

	inline bool init{ false };
}