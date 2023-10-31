#include "Vector3.h"

using namespace tpp;

Vector3::Vector3() : x(0), y(0), z(0)
{

}

Vector3::Vector3(double x, double y, double z) : x(x), y(y), z(z)
{

}

double Vector3::euclidean_distance(const Vector3& pos1, const Vector3& pos2)
{
	return
		sqrt(
			(pos1.x - pos2.x) * (pos1.x - pos2.x) +
			(pos1.y - pos2.y) * (pos1.y - pos2.y) +
			(pos1.z - pos2.z) * (pos1.z - pos2.z)
		);
}

double Vector3::manhattan_distance(const Vector3& pos1, const Vector3& pos2)
{
	return abs(pos1.x - pos2.x) + abs(pos1.y - pos2.y) + abs(pos1.z - pos2.z);
}

Vector3 Vector3::manhattan_distance_vec(const Vector3& pos1, const Vector3& pos2)
{
	return Vector3(
		abs(pos1.x - pos2.x),
		abs(pos1.y - pos2.y),
		abs(pos1.z - pos2.z)
	);
}

bool Vector3::operator==(const Vector3& other) const
{
	return
		this->x == other.x &&
		this->y == other.y &&
		this->z == other.z;
}

bool Vector3::operator!=(const Vector3& other) const
{
	return !(*this == other);
}

Vector3 Vector3::operator+(const Vector3& other) const
{
	return{ this->x + other.x, this->y + other.y, this->z + other.z };
}

Vector3 Vector3::operator-(const Vector3& other) const
{
	return{ this->x - other.x, this->y - other.y, this->z - other.z };
}

bool Vector3::operator<(const Vector3& other) const
{
	if (this->x < other.x)
		return true;
	if (this->x == other.x && this->y < other.y)
		return true;
	if (this->x == other.x && this->y == other.y && this->z < other.z)
		return true;
	return false;
}

Vector3 Vector3::operator*(int multiplier) const
{
	return{ this->x * multiplier, this->y * multiplier, this->z * multiplier };
}

std::ostream& tpp::operator<<(std::ostream& os, const Vector3& vector)
{
	os << vector.x << " " << vector.y << " " << vector.z;
	return os;
}

std::string Vector3::to_string() const
{
	return{ std::to_string((int)this->x) + " " + std::to_string((int)this->y) + " " + std::to_string((int)this->z) };
}

void Vector3::truncate()
{
	this->x = (int)this->x;
	this->y = (int)this->y;
	this->z = (int)this->z;
}

void Vector3::truncate2()
{
	this->truncate();

	if (this->x < 0) this->x--;
	if (this->y < 0) this->y--;
	if (this->z < 0) this->z--;
}

Vector3 Vector3::truncate2(const Vector3& vector)
{
	Vector3 result((int)vector.x, (int)vector.y, (int)vector.z);

	if (result.x < 0) result.x--;
	if (result.y < 0) result.y--;
	if (result.z < 0) result.z--;

	return result;
}