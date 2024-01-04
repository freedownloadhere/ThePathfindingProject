#include "AstarVector3.h"

using namespace tpp;

void AstarVector3::set_G(double value)
{
	this->G = value;
	this->F = this->G + this->H;
}

void AstarVector3::set_H(double value)
{
	this->H = value;
	this->F = this->G + this->H;
}

bool AstarVector3::operator()(const AstarVector3& lhs, const AstarVector3& rhs) const
{
	if (lhs.F > rhs.F)
		return true;

	else if (lhs.F == rhs.F && lhs.H > rhs.H)
		return true;

	return false;
}