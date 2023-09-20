#pragma once

#include "../Vector3.h"

namespace tpp
{
	struct AstarVector3 : public Vector3
	{
		double G{ 999999 }, H{ 0 }, F{ 0 };

		void setG(double value);
		void setH(double value);

		bool operator()(const AstarVector3& lhs, const AstarVector3& rhs) const;
	};
}