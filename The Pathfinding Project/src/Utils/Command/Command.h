#pragma once

#include <string>
#include <sstream>

#include "../../Pathfinder/Pathfinder.h"

namespace tpp
{
	class Command
	{
	public:
		Command(const std::string& command);

		static void init(const std::shared_ptr<Pathfinder>& pathfinder);
	private:
		inline static std::shared_ptr<Pathfinder> pathfinder{ nullptr };
		inline static bool mInit{ false };

		void tokenize(const std::string& command);
	};
}