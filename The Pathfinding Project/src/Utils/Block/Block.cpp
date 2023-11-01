#include "Block.h"

using namespace tpp;

bool Block::to_break() const
{
	return this->blocks_to_break.contains(this->id);
}

bool Block::to_open() const
{
	return this->blocks_to_open.contains(this->id);
}

bool Block::is_air() const
{
	return this->id == 0;
}

bool Block::is_walkable() const
{
	return !this->nonsolid.contains(this->id);
}