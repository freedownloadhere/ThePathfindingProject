#pragma once

#include <iostream>
#include <string>

namespace tpp
{
	struct Vector3
	{
		Vector3();

		Vector3(double x, double y, double z);

		double x{ 0 }, y{ 0 }, z{ 0 };

		static double euclideanDistance(const Vector3& pos1, const Vector3& pos2);

		static double manhattanDistance(const Vector3& pos1, const Vector3& pos2);

		static Vector3 manhattanDistanceVector(const Vector3& pos1, const Vector3& pos2);

		bool operator==(const Vector3& other) const;

		bool operator!=(const Vector3& other) const;

		Vector3 operator+(const Vector3& other) const;

		Vector3 operator-(const Vector3& other) const;

		Vector3 operator*(int multiplier) const;

		bool operator<(const Vector3& other) const;

		friend std::ostream& operator<<(std::ostream& os, const Vector3& vector);

		std::string toString() const;

		void truncate();

		/// <summary>
		/// Subtracts 1 from truncated negative values to
		/// solve an issue with the player position truncation.
		/// </summary>
		void truncate2();

		static Vector3 truncate2(const Vector3& vector);
	};
	const Vector3 nullvector(-566547550, -566547550, -566547550);
	const Vector3 zerovector(0, 0, 0);
	const Vector3 enumFacingVec[6] =
	{
		{0, -1, 0},	// DOWN
		{0, 1, 0},  // UP
		{0, 0, -1},	// NORTH
		{0, 0, 1},	// SOUTH
		{-1, 0, 0},	// WEST
		{1, 0, 0}	// EAST
	};
}