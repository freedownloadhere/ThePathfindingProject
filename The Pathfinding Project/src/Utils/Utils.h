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
	jfieldID getFieldID(JNIEnv* env, const jclass& cl, const char* name, const char* sig);
	jfieldID getStaticFieldID(JNIEnv* env, const jclass& cl, const char* name, const char* sig);
	jmethodID getMethodID(JNIEnv* env, const jclass& cl, const char* name, const char* sig);
	jobject getObject(JNIEnv* env, const jclass& cl, const jfieldID& fieldID);
	jobject getStaticObject(JNIEnv* env, const jclass& cl, const jfieldID& fieldID);
	jobject getObject(JNIEnv* env, const jclass& cl, const jclass& fieldIDcl, const char* fieldIDname, const char* fieldIDsig);
	jobject getStaticObject(JNIEnv* env, const jclass& cl, const jclass& fieldIDcl, const char* fieldIDname, const char* fieldIDsig);

	double clampAngle(double angle, double min, double max);
}