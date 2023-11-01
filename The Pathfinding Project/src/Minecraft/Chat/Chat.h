#pragma once

#include <iostream>

#include "../../JNI/JNI.h"
#include "../../Utils/Utils.h"

namespace tpp::chat
{
	bool initialize();

	bool send_msg_to_player(const std::string& message);
	bool send_msg_from_player(const std::string& message);
	std::string get_latest_msg();

	inline bool init{ false };
}