#include "ViewAngles.h"

using namespace tpp;

ViewAngles ViewAngles::operator+(const ViewAngles& other) const
{
	return{ this->yaw + other.yaw, this->pitch + other.pitch };
}

ViewAngles ViewAngles::operator-(const ViewAngles& other) const
{
	return{ this->yaw - other.yaw, this->pitch - other.pitch };
}

ViewAngles ViewAngles::operator/(int divisor) const
{
	return{ this->yaw / divisor, this->pitch / divisor };
}

ViewAngles ViewAngles::operator*(int multiplier) const
{
	return{ this->yaw * multiplier, this->pitch * multiplier };
}