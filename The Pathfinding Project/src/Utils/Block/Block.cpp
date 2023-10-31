#include "Block.h"

using namespace tpp;

bool Block::to_break() const
{
	return this->blocksToBreak.contains(this->id);
}

bool Block::to_open() const
{
	return this->blocksToOpen.contains(this->id);
}

bool Block::is_air() const
{
	return this->id == 0;
}

bool Block::is_walkable() const
{
	return !this->nonSolid.contains(this->id);
}