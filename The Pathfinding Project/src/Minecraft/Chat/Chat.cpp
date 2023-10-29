#include "Chat.h"

using namespace tpp;

bool tpp::chat::initialize(
	const jclass& mcClass,
	const jobject& mcClassInstance,
	const jclass& EntityPlayerSPClass,
	const jobject& mcThePlayerInstance
)
{
	tpp::chat::mcThePlayerInstance = mcThePlayerInstance;

	tpp::chat::chatCompClass = tpp::getClass(tpp::env, "net/minecraft/util/ChatComponentText");
	if (tpp::chat::chatCompClass == nullptr)
	{
		std::cout << "[-] Failed to get chat component class\n";
		return false;
	}

	tpp::chat::guiInGameClass = tpp::getClass(tpp::env, "net/minecraft/client/gui/GuiIngame");
	if (tpp::chat::guiInGameClass == nullptr)
	{
		std::cout << "[-] Could not get the GuiIngame class\n";
		return false;
	}

	tpp::chat::guiNewChatClass = tpp::getClass(tpp::env, "net/minecraft/client/gui/GuiNewChat");
	if (tpp::chat::guiNewChatClass == nullptr)
	{
		std::cout << "[-] Could not get the GuiNewChat class\n";
		return false;
	}

	tpp::chat::chatLineClass = tpp::getClass(tpp::env, "net/minecraft/client/gui/ChatLine");
	if (tpp::chat::chatLineClass == nullptr)
	{
		std::cout << "[-] Could not get the chatLine class\n";
		return false;
	}

	tpp::chat::listClass = tpp::getClass(tpp::env, "java/util/List");
	if (tpp::chat::listClass == nullptr)
	{
		std::cout << "[-] Could not get the List class\n";
		return false;
	}

	tpp::chat::addChatMessage = tpp::env->GetMethodID(EntityPlayerSPClass, "func_145747_a", "(Lnet/minecraft/util/IChatComponent;)V");
	if (tpp::chat::addChatMessage == nullptr)
	{
		std::cout << "[-] Failed to get addChatMessage method\n";
		return false;
	}

	tpp::chat::messageConstructor = tpp::env->GetMethodID(tpp::chat::chatCompClass, "<init>", "(Ljava/lang/String;)V");
	if (tpp::chat::messageConstructor == nullptr)
	{
		std::cout << "[-] Failed to get constructor for IChatComponent class\n";
		return false;
	}

	tpp::chat::sendChatMessage = tpp::env->GetMethodID(EntityPlayerSPClass, "func_71165_d", "(Ljava/lang/String;)V");
	if (tpp::chat::sendChatMessage == nullptr)
	{
		std::cout << "[-] Could not get the sendChatMessage method\n";
		return false;
	}

	auto ingameGuiFieldID = tpp::env->GetFieldID(mcClass, "field_71456_v", "Lnet/minecraft/client/gui/GuiIngame;");
	if (ingameGuiFieldID == nullptr)
	{
		std::cout << "[-] Could not get the ingameGui field\n";
		return false;
	}

	tpp::chat::ingameGuiInstance = tpp::env->GetObjectField(mcClassInstance, ingameGuiFieldID);
	if (tpp::chat::ingameGuiInstance == nullptr)
	{
		std::cout << "[-] Could not get the ingameGui object\n";
		return false;
	}

	auto persistentChatFieldID = tpp::env->GetFieldID(tpp::chat::guiInGameClass, "field_73840_e", "Lnet/minecraft/client/gui/GuiNewChat;");
	if (persistentChatFieldID == nullptr)
	{
		std::cout << "[-] Could not get the GuiNewChat field\n";
		return false;
	}

	tpp::chat::guiNewChatInstance = tpp::env->GetObjectField(tpp::chat::ingameGuiInstance, persistentChatFieldID);
	if (tpp::chat::guiNewChatInstance == nullptr)
	{
		std::cout << "[-] Could not get the GuiNewChat object\n";
		return false;
	}

	auto chatLinesField = tpp::env->GetFieldID(tpp::chat::guiNewChatClass, "field_146252_h", "Ljava/util/List;");
	if (chatLinesField == nullptr)
	{
		std::cout << "[-] Could not get the chatLines field\n";
		return false;
	}

	tpp::chat::chatLinesInstance = tpp::env->GetObjectField(tpp::chat::guiNewChatInstance, chatLinesField);
	if (tpp::chat::chatLinesInstance == nullptr)
	{
		std::cout << "[-] Could not get the chatLines object\n";
		return false;
	}

	tpp::chat::listGet = tpp::env->GetMethodID(tpp::chat::listClass, "get", "(I)Ljava/lang/Object;");
	if (tpp::chat::listGet == nullptr)
	{
		std::cout << "[-] Could not get the listGet method\n";
		return false;
	}

	tpp::chat::listSize = tpp::env->GetMethodID(tpp::chat::listClass, "size", "()I");
	if (tpp::chat::listSize == nullptr)
	{
		std::cout << "[-] Could not get the listSize method\n";
		return false;
	}

	tpp::chat::getChatComp = tpp::env->GetMethodID(tpp::chat::chatLineClass, "func_151461_a", "()Lnet/minecraft/util/IChatComponent;");
	if (tpp::chat::getChatComp == nullptr)
	{
		std::cout << "[-] Could not get the getChatComponent method\n";
		return false;
	}

	tpp::chat::getUnformattedText = tpp::env->GetMethodID(tpp::chat::chatCompClass, "func_150260_c", "()Ljava/lang/String;");
	if (tpp::chat::getUnformattedText == nullptr)
	{
		std::cout << "[-] Could not get the unformatted text method\n";
		return false;
	}

	return true;
}

bool tpp::chat::send_msg_to_player(const std::string& msg)
{
	jobject chatComp{ nullptr };
	jstring text{ nullptr };

	text = tpp::env->NewStringUTF(msg.c_str());

	chatComp = tpp::env->NewObject(chatCompClass, tpp::chat::messageConstructor, text);
	if (chatComp == nullptr)
	{
		std::cout << "Failed to create chat component object\n";
		return false;
	}

	tpp::env->CallVoidMethod(tpp::chat::mcThePlayerInstance, tpp::chat::addChatMessage, chatComp);

	tpp::env->DeleteLocalRef(text);
	tpp::env->DeleteLocalRef(chatComp);

	return true;
}

bool tpp::chat::send_msg_from_player(const std::string& msg)
{
	jstring text{ nullptr };

	text = tpp::env->NewStringUTF(msg.c_str());

	tpp::env->CallVoidMethod(tpp::chat::mcThePlayerInstance, tpp::chat::sendChatMessage, text);

	tpp::env->DeleteLocalRef(text);

	return true;
}

std::string tpp::chat::get_latest_msg()
{
	int size = tpp::env->CallIntMethod(tpp::chat::chatLinesInstance, tpp::chat::listSize);
	if (size == 0)
		return "No chat message to display";

	jobject lastChatLine = tpp::env->CallObjectMethod(tpp::chat::chatLinesInstance, tpp::chat::listGet, 0);
	if (lastChatLine == nullptr)
	{
		return "lastChatLine is a nullptr";
	}

	jobject chatComponent = tpp::env->CallObjectMethod(lastChatLine, tpp::chat::getChatComp);
	if (chatComponent == nullptr)
	{
		return "chatComponent is a nullptr";
	}

	jstring formattedText = (jstring)tpp::env->CallObjectMethod(chatComponent, tpp::chat::getUnformattedText);
	if (formattedText == nullptr)
	{
		return "formattedText is a nullptr";
	}

	auto result = tpp::env->GetStringUTFChars(formattedText, 0);

	return result;
}