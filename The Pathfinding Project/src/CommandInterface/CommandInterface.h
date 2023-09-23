#pragma once

#include <string>
#include <sstream>

#include "../Instance/Instance.h"

namespace tpp
{
	class CommandInterface
	{
	public:
		CommandInterface(const std::shared_ptr<Instance>& instance);

		void enterLoop();

	private:
		std::shared_ptr<Instance> mInstance{ nullptr };
		std::string mCmdName, mCmdArgs;
		bool mCmdResult{ false };

		bool cmdGoto();
		bool cmdMakePath();
		bool cmdPrint();
		bool cmdSend();
		bool cmdWait();
		bool cmdEcho();

		bool mEcho{ true }, mInit{ false };
		bool init(const std::shared_ptr<Instance>& instance);
	};
}