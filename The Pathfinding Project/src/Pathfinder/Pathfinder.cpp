#include "Pathfinder.h"

using namespace tpp;
using namespace std::literals::chrono_literals;

namespace std
{
	template<>
	struct hash<Vector3>
	{
		size_t operator()(const Vector3& key) const
		{
			return std::hash<double>()(key.x - key.y + key.z);
		}
	};

	template<>
	struct hash<AstarVector3>
	{
		size_t operator()(const AstarVector3& key) const
		{
			return std::hash<double>()(key.x - key.y + key.z - key.F + key.H);
		}
	};
}

Pathfinder::Pathfinder(const std::shared_ptr<Minecraft>& minecraft)
{
	this->mInit = this->init(minecraft);

	if (!this->mInit)
		std::cout << "Failed to initialize Pathfinder class\n";
}

bool Pathfinder::init(const std::shared_ptr<Minecraft>& minecraft)
{
	this->minecraft = minecraft;

	this->wasd[0][0].type = INPUT_KEYBOARD;
	this->wasd[0][0].ki.wVk = 0x57;
	this->wasd[0][1].type = INPUT_KEYBOARD;
	this->wasd[0][1].ki.wVk = 0x57;
	this->wasd[0][1].ki.dwFlags = KEYEVENTF_KEYUP;

	this->wasd[1][0].type = INPUT_KEYBOARD;
	this->wasd[1][0].ki.wVk = 0x41;
	this->wasd[1][1].type = INPUT_KEYBOARD;
	this->wasd[1][1].ki.wVk = 0x41;
	this->wasd[1][1].ki.dwFlags = KEYEVENTF_KEYUP;

	this->wasd[2][0].type = INPUT_KEYBOARD;
	this->wasd[2][0].ki.wVk = 0x53;
	this->wasd[2][1].type = INPUT_KEYBOARD;
	this->wasd[2][1].ki.wVk = 0x53;
	this->wasd[2][1].ki.dwFlags = KEYEVENTF_KEYUP;

	this->wasd[3][0].type = INPUT_KEYBOARD;
	this->wasd[3][0].ki.wVk = 0x44;
	this->wasd[3][1].type = INPUT_KEYBOARD;
	this->wasd[3][1].ki.wVk = 0x44;
	this->wasd[3][1].ki.dwFlags = KEYEVENTF_KEYUP;

	return true;
}

bool Pathfinder::isInit()
{
	return this->mInit;
}

bool Pathfinder::listContains(
	const AstarVector3& element,
	const std::vector<AstarVector3>& heap
)
{
	for (const auto& i : heap)
	{
		if (i == element)
			return true;
	}
	return false;
}

bool Pathfinder::isWalkable(const AstarVector3& coordinates)
{
	Vector3 v[3] = { coordinates, coordinates + this->upOne, coordinates + this->upTwo };

	for (const auto& i : v)
		if (!this->walkableBlockCache.contains(i))
			this->walkableBlockCache[i] = !Block::nonSolid.contains(this->minecraft->world->getBlockID(i));

	return(
		this->walkableBlockCache[v[0]] && !this->walkableBlockCache[v[1]] && !this->walkableBlockCache[v[2]]
		);
}

bool Pathfinder::makePath(Vector3 start, Vector3 target, int flags, const std::string& blockToSet)
{
	if (start.y < 0)
	{
		std::cout << "[makePath] Invalid start block " << start << "\n";
		return false;
	}

	if (target.y < 0)
	{
		std::cout << "[makePath] Invalid target block " << target << "\n";
		return false;
	}

	if (flags & (int)MakePathFlags::SAFE)
	{
		while
			(
				start.y >= 0 &&
				Block::nonSolid.contains(this->minecraft->world->getBlockID(start))
				)
			start.y--;

		while
			(
				target.y >= 0 &&
				Block::nonSolid.contains(this->minecraft->world->getBlockID(target))
				)
			target.y--;
	}

	if (start.y < 0)
	{
		std::cout << "[makePath] Could not find a valid (solid) start block!\n";
		return false;
	}

	if (target.y < 0)
	{
		std::cout << "[makePath] Could not find a valid (solid) target block!\n";
		return false;
	}

	if (!(flags & (int)MakePathFlags::USEPREVCACHE))
		this->walkableBlockCache.clear();

	std::list<Vector3> path = this->defaultAstar(start, target);
	if (path.empty())
	{
		std::cout << "[makePath] Failed to make a path to " << target << ".\n";
		return false;
	}

	if (flags & (int)MakePathFlags::SETBLOCK)
	{
		for (const auto& i : path)
		{
			this->minecraft->chat->sendMessageFromPlayer(
				"/setblock " + std::to_string(i.x) + " " + std::to_string(i.y) + " " + std::to_string(i.z) + " " + blockToSet
			);
			std::this_thread::sleep_for(10ms);
		}
	}

	//this->traversePath(path);

	return true;
}

bool Pathfinder::goTo(Vector3 target, int flags, const std::string& blockToSet)
{
	return this->makePath(
		this->minecraft->player->getBlockBelowPosition(), 
		target, 
		flags, 
		blockToSet
	);
}

std::list<Vector3> Pathfinder::defaultAstar(const Vector3& start, const Vector3& target)
{

	std::vector<AstarVector3> heapToSearch;
	heapToSearch.reserve(500);

	std::unordered_set<Vector3> processed;
	std::unordered_map<AstarVector3, AstarVector3> connections;

	AstarVector3 current{ start };
	current.setG(0);
	current.setH(Vector3::manhattanDistance(current, target));

	heapToSearch.emplace_back(current);

	while (!heapToSearch.empty())
	{
		current = heapToSearch.front();

		std::pop_heap(heapToSearch.begin(), heapToSearch.end(), AstarVector3());
		heapToSearch.pop_back();
		processed.insert(current);

		if (current == target)
		{
			std::list<Vector3> result;

			while (current != start)
			{
				result.emplace_front(current.x, current.y, current.z);
				current = connections[current];
			}
			result.emplace_front(current.x, current.y, current.z);

			return result;
		}

		for (const auto& k : this->directionalVector)
		{
			AstarVector3 neighbour{ current + k };
			double distanceToNeighbour = (current.y == neighbour.y ? current.G + 10 : current.G + 14);

			if (!this->isWalkable(neighbour))
				continue;

			if (processed.contains(neighbour))
				continue;

			if (neighbour.G > distanceToNeighbour)
			{
				neighbour.setG(distanceToNeighbour);
				connections[neighbour] = current;
			}

			if (!this->listContains(neighbour, heapToSearch))
			{
				neighbour.setH(Vector3::manhattanDistance(current, target));
				heapToSearch.emplace_back(neighbour);
				std::push_heap(heapToSearch.begin(), heapToSearch.end(), AstarVector3());
			}
		}
	}

	return {};
}

std::vector<std::pair<Vector3, int>> Pathfinder::makeWalkMap(const std::list<Vector3>& path)
{
	std::list<Vector3>::const_iterator it1, it2;
	it1 = it2 = path.begin();
	it2++;

	std::vector<std::pair<Vector3, int>> walkMap;
	walkMap.reserve(path.size());

	while (it2 != path.end() && it1 != path.end())
	{
		Vector3 delta{ *it2 - *it1 };
		int deltaEquals{ 0 };

		for (; deltaEquals < 6; ++deltaEquals)
			if (delta == tpp::enumFacingVec[deltaEquals])
				break;

		if (deltaEquals == 6)
		{
			std::cout << "[MakeWalkMap] DeltaEquals is invalid (huh?) (delta is " << delta << ")\n";
			return {};
		}

		if (walkMap.empty() || walkMap.back().second != deltaEquals)
			walkMap.emplace_back(*it1 + this->getBlockGoalpoint, deltaEquals);

		it1++;
		it2++;
	}

	walkMap.emplace_back(path.back(), 0);

	return walkMap;
}

void Pathfinder::traversePath(const std::list<Vector3>& path)
{
	// Assuming that player pos = first element in path.

	const auto walkMap{ this->makeWalkMap(path) };

	if (walkMap.empty())
		return;

	auto iterator = walkMap.begin();

	while (iterator != walkMap.end() && !GetAsyncKeyState(VK_NUMPAD0))
	{
		auto distance = Vector3::euclideanDistance(this->minecraft->player->getFootPosition(), iterator->first);

		if (distance <= this->errorMargin)
		{
			this->minecraft->player->setViewAngles(tpp::enumFacingToViewAngles[iterator->second]);
			//SendInput(1, &this->wasd[0][0], sizeof(INPUT));
			iterator++;
		}

		std::this_thread::sleep_for(10ms);
	}

	//SendInput(1, &this->wasd[0][1], sizeof(INPUT));
}