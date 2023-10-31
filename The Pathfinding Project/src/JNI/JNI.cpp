#include "JNI.h"

using namespace tpp;



bool jni::initialize()
{
	JavaVM* jvm{ nullptr };

	if (JNI_GetCreatedJavaVMs(&jvm, 1, nullptr) != 0)
		return false;

	jvm->AttachCurrentThread((void**)&env, nullptr);
	if (env == nullptr)
		return false;


}