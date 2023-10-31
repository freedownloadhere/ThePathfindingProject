#include "Chat.h"

using namespace tpp;

namespace tpp::chat
{
	static jobject
		mcThePlayerInstance{ nullptr },
		ingameGuiInstance{ nullptr },
		guiNewChatInstance{ nullptr },
		chatLinesInstance{ nullptr };
}

bool chat::initialize(
	const jobject& mcClassInstance,
	const jobject& mcThePlayerInstance
)
{
	chat::mcThePlayerInstance = mcThePlayerInstance;

	ingameGuiInstance = jni::get_obj(mcClassInstance, "Minecraft", "ingameGUI");
	if (ingameGuiInstance == nullptr)
	{
		std::cout << "[-] Could not get the ingameGui object\n";
		return false;
	}

	guiNewChatInstance = jni::get_obj(ingameGuiInstance, "GuiIngame", "persistantChatGUI");
	if (guiNewChatInstance == nullptr)
	{
		std::cout << "[-] Could not get the GuiNewChat object\n";
		return false;
	}

	chatLinesInstance = jni::get_obj(guiNewChatInstance, "GuiNewChat", "chatLines");
	if (chatLinesInstance == nullptr)
	{
		std::cout << "[-] Could not get the chatLines object\n";
		return false;
	}

	init = true;
	return true;
}

bool chat::send_msg_to_player(const std::string& msg)
{
	jobject chatComp{ nullptr };
	jstring text{ nullptr };

	text = env->NewStringUTF(msg.c_str());

	chatComp = jni::new_obj("ChatComponentText", "<init>", text);
	if (chatComp == nullptr)
	{
		std::cout << "Failed to create chat component object\n";
		return false;
	}

	jni::call_void(mcThePlayerInstance, "EntityPlayerSP", "addChatMessage", chatComp);

	env->DeleteLocalRef(text);
	env->DeleteLocalRef(chatComp);

	return true;
}

bool chat::send_msg_from_player(const std::string& msg)
{
	jstring text{ nullptr };

	text = env->NewStringUTF(msg.c_str());

	jni::call_void(mcThePlayerInstance, "EntityPlayerSP", "sendChatMessage", text);

	env->DeleteLocalRef(text);

	return true;
}

std::string chat::get_latest_msg()
{
	int size = jni::call_int(chatLinesInstance, "List", "size");
	if (size == 0)
		return "No chat message to display";

	jobject lastChatLine = jni::call_obj(chatLinesInstance, "List", "get", 0);
	if (lastChatLine == nullptr)
	{
		return "lastChatLine is a nullptr";
	}

	jobject chatComponent = jni::call_obj(lastChatLine, "ChatLine", "getChatComponent");
	if (chatComponent == nullptr)
	{
		return "chatComponent is a nullptr";
	}

	jstring formattedText = (jstring)jni::call_obj(chatComponent, "ChatComponentText", "getUnformattedText");
	if (formattedText == nullptr)
	{
		return "formattedText is a nullptr";
	}

	auto result = env->GetStringUTFChars(formattedText, 0);

	return result;
}