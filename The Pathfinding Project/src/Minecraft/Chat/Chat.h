#pragma once

#include <iostream>
#include <jni.h>

#include "../../Utils/Utils.h"

namespace tpp
{
	class Chat
	{
	public:
		Chat(
			JNIEnv* env,
			const jclass& mcClass,
			const jobject& mcClassInstance,
			const jclass& EntityPlayerSPClass,
			const jobject& mcThePlayerInstance
		);

		void sendMessageToPlayer(const std::string& message);
		void sendMessageFromPlayer(const std::string& message);
		std::string getLatestChatMessage();
		bool isInit();

	private:
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

		bool mInit{ false };
		JNIEnv* env{ nullptr };
		bool init(
			JNIEnv* env,
			const jclass& mcClass,
			const jobject& mcClassInstance,
			const jclass& EntityPlayerSPClass,
			const jobject& mcThePlayerInstance
		);
	};
}