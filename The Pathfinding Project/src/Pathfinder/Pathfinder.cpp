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

static std::vector<Vector3> directions =
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
	up_one{ 0, 1, 0 },
	up_two{ 0, 2, 0 },
	block_goal_offset{ 0.5, 1, 0.5 };

static constexpr double error_margin{ 0.3 };

static std::unordered_map<Vector3, bool> block_cache;

bool tpp::pathfinder::initialize()
{
	init = true;
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
	Vector3 v[3] = { coordinates, coordinates + up_one, coordinates + up_two };

	for (const auto& i : v)
		if (!block_cache.contains(i))
			block_cache[i] = !Block::nonsolid.contains(tpp::world::get_block_id(i));

	return block_cache[v[0]] && !block_cache[v[1]] && !block_cache[v[2]];
}

bool tpp::pathfinder::make_path(Vector3 start, Vector3 target, int flags, const std::string& blockToSet)
{
	if (start.y < 0)
	{
		std::cout << "[make_path] Invalid start block " << start << "\n";
		return false;
	}

	if (target.y < 0)
	{
		std::cout << "[make_path] Invalid target block " << target << "\n";
		return false;
	}

	if (flags & (int)MakePathFlags::SAFE)
	{
		while
			(
				start.y >= 0 &&
				Block::nonsolid.contains(tpp::world::get_block_id(start))
				)
			start.y--;

		while
			(
				target.y >= 0 &&
				Block::nonsolid.contains(tpp::world::get_block_id(target))
				)
			target.y--;
	}

	if (start.y < 0)
	{
		std::cout << "[make_path] Could not find a valid (solid) start block!\n";
		return false;
	}

	if (target.y < 0)
	{
		std::cout << "[make_path] Could not find a valid (solid) target block!\n";
		return false;
	}

	if (!(flags & (int)MakePathFlags::USEPREVCACHE))
		block_cache.clear();

	std::list<Vector3> path = default_astar(start, target);
	if (path.empty())
	{
		std::cout << "[make_path] Failed to make a path to " << target << ".\n";
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
	std::vector<AstarVector3> search_heap;
	search_heap.reserve(500);

	std::unordered_set<Vector3> processed;
	std::unordered_map<AstarVector3, AstarVector3> connections;

	AstarVector3 current{ start };
	current.set_G(0);
	current.set_H(Vector3::manhattan_distance(current, target));

	search_heap.emplace_back(current);

	while (!search_heap.empty())
	{
		current = search_heap.front();
		std::pop_heap(search_heap.begin(), search_heap.end(), AstarVector3());
		search_heap.pop_back();
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

		for (const auto& k : directions)
		{
			AstarVector3 next{ current + k };
			double distance_to_next = (current.y == next.y ? current.G + 10 : current.G + 14);

			if (!is_walkable(next))
				continue;

			if (processed.contains(next))
				continue;

			if (next.G > distance_to_next)
			{
				next.set_G(distance_to_next);
				connections[next] = current;
			}

			if (!list_contains(next, search_heap))
			{
				next.set_H(Vector3::manhattan_distance(current, target));
				search_heap.emplace_back(next);
				std::push_heap(search_heap.begin(), search_heap.end(), AstarVector3());
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

	std::vector<std::pair<Vector3, int>> navmap;
	navmap.reserve(path.size());

	while (it2 != path.end() && it1 != path.end())
	{
		Vector3 delta{ *it2 - *it1 };
		int indexof{ 0 };

		for (; indexof < 6; ++indexof)
			if (delta == tpp::enum_facing_vec[indexof])
				break;

		if (indexof == 6)
		{
			std::cout << "[make_navmap] Index is invalid (delta is " << delta << ")\n";
			return {};
		}

		if (navmap.empty() || navmap.back().second != indexof)
			navmap.emplace_back(*it1 + block_goal_offset, indexof);

		it1++;
		it2++;
	}

	navmap.emplace_back(path.back(), 0);

	return navmap;
}