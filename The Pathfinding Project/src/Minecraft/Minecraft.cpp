#include "Minecraft.h"

using namespace tpp;

bool minecraft::initialize()
{
	JavaVM* jvm{ nullptr };

	if (JNI_GetCreatedJavaVMs(&jvm, 1, nullptr) != 0)
	{
		std::cout << "[-] Failed to get created Java VMs!\n";
		return false;
	}

	jvm->AttachCurrentThread((void**)&env, nullptr);
	if (env == nullptr)
	{
		std::cout << "[-] Failed to attach current JVM thread!\n";
		return false;
	}

	minecraft::mcClass = get_class(env, "net/minecraft/client/Minecraft");
	if (minecraft::mcClass == nullptr)
	{
		std::cout << "[-] Failed to find class Minecraft!\n";
		std::cout << "[INFO] Are you using MC Forge 1.8.9?\n";
		return false;
	}

	jfieldID fieldID{ env->GetStaticFieldID(minecraft::mcClass, "field_71432_P", "Lnet/minecraft/client/Minecraft;") };
	if (fieldID == nullptr)
	{
		std::cout << "[-] Failed to get static field ID theMinecraft!\n";
		return false;
	}

	minecraft::mcClassInstance = env->GetStaticObjectField(minecraft::mcClass, fieldID);
	if (minecraft::mcClassInstance == nullptr)
	{
		std::cout << "[-] Failed to get static object field theMinecraft!\n";
		return false;
	}

	player::initialize(
		mcClass,
		mcClassInstance
	);
	if (!player::init) return false;

	world::initialize(
		mcClass,
		mcClassInstance
	);
	if (!world::init) return false;

	chat::initialize(
		mcClass,
		mcClassInstance,
		player::EntityPlayerSPClass,
		player::mcThePlayerInstance
	);
	if (!chat::init) return false;

	return true;
}