#include "Utils.h"

using namespace tpp;

jclass tpp::getClass(JNIEnv* env, const std::string& path)
{
	static std::unordered_map<std::string, jclass> classMap;

	if (classMap.empty())
	{
		JavaVM* jvm = nullptr;
		env->GetJavaVM(&jvm);
		jvmtiEnv* tienv = nullptr;
		jvm->GetEnv((void**)&tienv, JVMTI_VERSION_1_2);
		jint classCount = 0;
		jclass* classes = nullptr;
		tienv->GetLoadedClasses(&classCount, &classes);
		jclass ClassClass = env->FindClass("java/lang/Class");
		jmethodID getName = env->GetMethodID(ClassClass, "getName", "()Ljava/lang/String;");

		for (int i = 0; i < classCount; ++i)
		{
			jstring name = (jstring)env->CallObjectMethod(classes[i], getName);
			const char* chars = env->GetStringUTFChars(name, nullptr);
			jsize size = env->GetStringUTFLength(name);
			std::string className(chars, size);
			env->ReleaseStringUTFChars(name, chars);
			for (char& character : className)
			{
				if (character == '.') character = '/';
			}
			classMap.insert({ className, classes[i] });
		}
		tienv->Deallocate((unsigned char*)classes);
	}

	jclass foundclass = nullptr;
	try
	{
		foundclass = classMap.at(path);
	}
	catch (...)
	{
		std::cerr << "[-] Failed to find class " + path << std::endl;
	}
	return foundclass;
}

double tpp::clampAngle(double angle, double min, double max)
{
	if (angle >= 90)
		angle -= 360;

	return std::clamp<double>(angle, min, max);
}