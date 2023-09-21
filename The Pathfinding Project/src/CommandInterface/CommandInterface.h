#pragma once

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
		std::string mLastInput;

		bool mInit{ false };
		bool init(const std::shared_ptr<Instance>& instance);
	};
}