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

static const Vector3 directionalVector[12] =
{
	{0, 0, -1}, // NORTH
	{1, 0, 0},  // EAST
	{0, 0, 1},  // SOUTH
	{-1, 0, 0}, // WEST
	{0, -1, -1}, // NORTH + DOWN
	{1, -1, 0},  // EAST + DOWN
	{0, -1, 1},  // SOUTH + DOWN
	{-1, -1, 0}, // WEST + DOWN
	{0, 1, -1}, // NORTH + UP
	{1, 1, 0},  // EAST + UP
	{0, 1, 1},  // SOUTH + UP
	{-1, 1, 0}, // WEST + UP
};

static const Vector3
	upOne{ 0, 1, 0 },
	upTwo{ 0, 2, 0 },
	getBlockGoalpoint{ 0.5, 1, 0.5 };

static constexpr double errorMargin{ 0.3 };

static std::unordered_map<Vector3, bool> walkableBlockCache;

bool tpp::pathfinder::initialize()
{
	return true;
}

bool tpp::pathfinder::list_contains(
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

bool tpp::pathfinder::is_walkable(const AstarVector3& coordinates)
{
	Vector3 v[3] = { coordinates, coordinates + upOne, coordinates + upTwo };

	for (const auto& i : v)
		if (!walkableBlockCache.contains(i))
			walkableBlockCache[i] = !Block::nonSolid.contains(tpp::world::get_block_id(i));

	return(
		walkableBlockCache[v[0]] && !walkableBlockCache[v[1]] && !walkableBlockCache[v[2]]
		);
}

bool tpp::pathfinder::make_path(Vector3 start, Vector3 target, int flags, const std::string& blockToSet)
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
				Block::nonSolid.contains(tpp::world::get_block_id(start))
				)
			start.y--;

		while
			(
				target.y >= 0 &&
				Block::nonSolid.contains(tpp::world::get_block_id(target))
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
		walkableBlockCache.clear();

	std::list<Vector3> path = default_astar(start, target);
	if (path.empty())
	{
		std::cout << "[makePath] Failed to make a path to " << target << ".\n";
		return false;
	}

	if (flags & (int)MakePathFlags::SETBLOCK)
	{
		for (const auto& i : path)
		{
			tpp::chat::send_msg_from_player(
				"/setblock " + std::to_string(i.x) + " " + std::to_string(i.y) + " " + std::to_string(i.z) + " " + blockToSet
			);
			std::this_thread::sleep_for(10ms);
		}
	}

	if (flags & (int)MakePathFlags::TRAVERSE)
	{
		traverse_path(path);
	}

	return true;
}

bool tpp::pathfinder::go_to(Vector3 target, int flags, const std::string& blockToSet)
{
	return make_path(
		tpp::player::get_below_pos(), 
		target, 
		flags, 
		blockToSet
	);
}

std::list<Vector3> tpp::pathfinder::default_astar(const Vector3& start, const Vector3& target)
{
	std::vector<AstarVector3> heapToSearch;
	heapToSearch.reserve(500);

	std::unordered_set<Vector3> processed;
	std::unordered_map<AstarVector3, AstarVector3> connections;

	AstarVector3 current{ start };
	current.set_G(0);
	current.set_H(Vector3::manhattan_distance(current, target));

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

		for (const auto& k : directionalVector)
		{
			AstarVector3 neighbour{ current + k };
			double distanceToNeighbour = (current.y == neighbour.y ? current.G + 10 : current.G + 14);

			if (!is_walkable(neighbour))
				continue;

			if (processed.contains(neighbour))
				continue;

			if (neighbour.G > distanceToNeighbour)
			{
				neighbour.set_G(distanceToNeighbour);
				connections[neighbour] = current;
			}

			if (!list_contains(neighbour, heapToSearch))
			{
				neighbour.set_H(Vector3::manhattan_distance(current, target));
				heapToSearch.emplace_back(neighbour);
				std::push_heap(heapToSearch.begin(), heapToSearch.end(), AstarVector3());
			}
		}
	}

	return {};
}

std::vector<std::pair<Vector3, int>> tpp::pathfinder::make_navmap(const std::list<Vector3>& path)
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
			walkMap.emplace_back(*it1 + getBlockGoalpoint, deltaEquals);

		it1++;
		it2++;
	}

	walkMap.emplace_back(path.back(), 0);

	return walkMap;
}

void tpp::pathfinder::traverse_path(const std::list<Vector3>& path)
{
	// Assuming that player pos = first element in path.

	const auto walkMap{ make_navmap(path) };

	if (walkMap.empty())
		return;

	auto iterator = walkMap.begin();

	while (iterator != walkMap.end() && !GetAsyncKeyState(VK_NUMPAD0))
	{
		auto distance = Vector3::euclidean_distance(tpp::player::get_foot_pos(), iterator->first);

		if (distance <= errorMargin)
		{
			tpp::player::set_viewangles(tpp::enumFacingToViewAngles[iterator->second]);
			//SendInput(1, &this->wasd[0][0], sizeof(INPUT));
			iterator++;
		}

		std::this_thread::sleep_for(10ms);
	}

	//SendInput(1, &this->wasd[0][1], sizeof(INPUT));
}