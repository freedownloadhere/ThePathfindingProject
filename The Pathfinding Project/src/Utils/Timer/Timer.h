#pragma once

#include <iostream>
#include <chrono>

namespace tpp
{
	struct Timer
	{
		std::chrono::steady_clock::time_point start, end;
		const char* topic;

		Timer();
		Timer(const char* topic);
		~Timer();
	};
}