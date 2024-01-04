#include "Chat.h"

using namespace tpp;

static jobject chat_list{ nullptr };

bool chat::initialize()
{
	chat_list = jni::get_obj("GuiNewChat", "chatLines");
	if (chat_list == nullptr)
	{
		std::cout << "[-] Could not get the chat_list object\n";
		return false;
	}

	init = true;
	return true;
}

bool chat::send_msg_to_player(const std::string& msg)
{
	jobject chat_comp{ nullptr };
	jstring text{ nullptr };

	text = env->NewStringUTF(msg.c_str());

	chat_comp = jni::new_obj("ChatComponentText", "<init>", text);
	if (chat_comp == nullptr)
	{
		std::cout << "Failed to create chat component object\n";
		return false;
	}

	jni::call_void("EntityPlayerSP", "addChatMessage", chat_comp);

	env->DeleteLocalRef(text);
	env->DeleteLocalRef(chat_comp);

	return true;
}

bool chat::send_msg_from_player(const std::string& msg)
{
	jstring text{ nullptr };

	text = env->NewStringUTF(msg.c_str());

	jni::call_void("EntityPlayerSP", "sendChatMessage", text);

	env->DeleteLocalRef(text);

	return true;
}

std::string chat::get_latest_msg()
{
	int size = jni::call_int(chat_list, "List", "size");
	if (size == 0)
		return "No chat message to display";

	jobject last_chatline = jni::call_obj(chat_list, "List", "get", 0);
	if (last_chatline == nullptr)
	{
		return "last_chatline is a nullptr";
	}

	jobject chat_component = jni::call_obj(last_chatline, "ChatLine", "getChatComponent");
	if (chat_component == nullptr)
	{
		return "chat_component is a nullptr";
	}

	jstring formatted_text = jni::call_str(chat_component, "ChatComponentText", "getUnformattedText");
	if (formatted_text == nullptr)
	{
		return "formatted_text is a nullptr";
	}

	auto result = env->GetStringUTFChars(formatted_text, 0);

	return result;
}