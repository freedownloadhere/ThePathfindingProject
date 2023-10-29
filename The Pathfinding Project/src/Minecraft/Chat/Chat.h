#pragma once

#include <iostream>

#include "../../JNI/JNI.h"
#include "../../Utils/Utils.h"

namespace tpp::chat
{
	bool initialize(
		const jclass& mcClass,
		const jobject& mcClassInstance,
		const jclass& EntityPlayerSPClass,
		const jobject& mcThePlayerInstance
	);

	bool send_msg_to_player(const std::string& message);
	bool send_msg_from_player(const std::string& message);
	std::string get_latest_msg();

	bool init{ false };

	jclass
		guiInGameClass{ nullptr },
		guiNewChatClass{ nullptr },
		chatLineClass{ nullptr },
		listClass{ nullptr },
		chatCompClass{ nullptr };
	jobject
		mcThePlayerInstance{ nullptr },
		ingameGuiInstance{ nullptr },
		guiNewChatInstance{ nullptr },
		chatLinesInstance{ nullptr };
	jmethodID
		addChatMessage{ nullptr },
		sendChatMessage{ nullptr },
		messageConstructor{ nullptr },
		listSize{ nullptr },
		getUnformattedText{ nullptr },
		listGet{ nullptr },
		getChatComp{ nullptr };
}