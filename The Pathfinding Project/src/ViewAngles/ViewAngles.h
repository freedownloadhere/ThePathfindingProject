#pragma once

namespace tpp
{
	struct ViewAngles
	{
		double yaw{ 0 }, pitch{ 0 };

		ViewAngles operator+(const ViewAngles& other) const;

		ViewAngles operator-(const ViewAngles& other) const;

		ViewAngles operator/(int divisor) const;

		ViewAngles operator*(int multiplier) const;
	};
	const ViewAngles enumFacingToViewAngles[6] =
	{
		{0, -90},
		{0, 90},
		{180, 0},
		{0, 0},
		{90, 0},
		{-90, 0}
	};
}