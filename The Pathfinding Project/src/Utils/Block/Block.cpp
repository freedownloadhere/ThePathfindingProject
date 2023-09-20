#include "Block.h"

using namespace tpp;

bool Block::toBreak() const
{
	return this->blocksToBreak.contains(this->id);
}

bool Block::toOpen() const
{
	return this->blocksToOpen.contains(this->id);
}

bool Block::isAir() const
{
	return this->id == 0;
}

bool Block::isWalkable() const
{
	return !this->nonSolid.contains(this->id);
}