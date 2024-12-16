#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <set>

enum direction_t { E_NORTH, E_EAST, E_SOUTH, E_WEST };

class AoC2024_day06 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::set<coord_str> map_, visited_;
	coord_str guard_pos_, map_size_;
	direction_t guard_head_;
	uint64_t get_guards_distinct_positions_count();
	uint64_t get_guard_loops_obstacles_count();
	bool trace_guard_loop(const coord_str obstacle);
	bool still_in_map(const coord_str position) const;
	coord_str get_step_from_direction(const direction_t direction);
	direction_t guard_turn(const direction_t direction);
};

bool AoC2024_day06::init(const std::vector<std::string> lines) {
	bool guard_found = false;
	uint32_t size = 0;

	map_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (i) {
			if (size != lines[i].size()) {
				std::cout << "Invalid map size at line " << i + 1 << std::endl;
				return false;
			}
		} else {
			size = lines[i].size();
		}

		if (lines[i].find_first_not_of("#.^") != std::string::npos) {
			std::cout << "Invalid map content at line " << i + 1 << std::endl;
			return false;
		}

		for (uint32_t j = 0; j < lines[i].size(); j++) {
			switch (lines[i][j]) {
				case '#':
					map_.emplace(coord_str(j, i));
					break;
				case '^':
					guard_found = true;
					guard_pos_ = coord_str(j, i);
					guard_head_ = E_NORTH;
					break;
				case '.':
				default:
					break;
			}
		}
	}

	if (guard_found) {
		map_size_ = coord_str(size, lines.size());
		return true;
	} else {
		std::cout << "Guard position not found in map" << std::endl;
		return false;
	}
}

coord_str AoC2024_day06::get_step_from_direction(const direction_t direction) {
	switch (direction) {
		case E_NORTH:
			return coord_step_north;

		case E_EAST:
			return coord_step_east;

		case E_SOUTH:
			return coord_step_south;

		case E_WEST:
			return coord_step_west;

		default:
			assert(false);
			break;
	}
}

direction_t AoC2024_day06::guard_turn(direction_t direction) {
	switch (direction) {
		case E_NORTH:
			return E_EAST;
			break;

		case E_EAST:
			return E_SOUTH;
			break;

		case E_SOUTH:
			return E_WEST;
			break;

		case E_WEST:
			return E_NORTH;
			break;

		default:
			assert(false);
			break;
	}
}

bool AoC2024_day06::still_in_map(const coord_str position) const {
	return ((position.x >= 0) && (position.x < map_size_.x) && (position.y >= 0) && (position.y < map_size_.y));
}

bool AoC2024_day06::trace_guard_loop(const coord_str obstacle) {
	coord_str pos, next, tmp;
	std::set<coord_3d_str> history;
	coord_3d_str step;
	direction_t direction;

	if ((guard_pos_ == obstacle) || map_.count(obstacle)) {
		return false;
	}

	history.clear();
	pos = guard_pos_;
	direction = guard_head_;
	next = get_step_from_direction(direction);

	while (still_in_map(pos)) {

		tmp = pos + next;
		if (map_.count(tmp) || (tmp == obstacle)) {

			direction = guard_turn(direction);

			next = get_step_from_direction(direction);
		} else {

			step = {tmp.x, tmp.y, direction};

			if (history.count(step)) {
				return true;
			} else {
				pos = tmp;
				history.emplace(step);
			}
		}
	}

	return false;
}

uint64_t AoC2024_day06::get_guard_loops_obstacles_count() {
	uint64_t result = 0;

	for (auto& coord : visited_) {
		if (trace_guard_loop(coord)) {
			result++;
		}
	}

	return result;
}

uint64_t AoC2024_day06::get_guards_distinct_positions_count() {
	coord_str pos, step, tmp;
	direction_t head;

	visited_.clear();

	pos = guard_pos_;
	head = guard_head_;
	step = get_step_from_direction(head);

	while (still_in_map(pos)) {
		visited_.emplace(pos);

		tmp = pos + step;
		if (map_.count(tmp)) {

			head = guard_turn(head);

			step = get_step_from_direction(head);
		} else {
			pos = tmp;
		}
	}

	return visited_.size();
}

int32_t AoC2024_day06::get_aoc_day() {
	return 6;
}

int32_t AoC2024_day06::get_aoc_year() {
	return 2024;
}

void AoC2024_day06::tests() {
	uint64_t result;

	if (init({"....#.....", ".........#", "..........", "..#.......", ".......#..", "..........", ".#..^.....", "........#.", "#.........", "......#..."})) {
		result = get_guards_distinct_positions_count(); // 41
		result = get_guard_loops_obstacles_count();		// 6; {{3, 6}, {6, 7}, {7, 7}, {1, 8}, {3, 8}, {7, 9}}
	}
}

bool AoC2024_day06::part1() {
	int64_t result = 0;

	result = get_guards_distinct_positions_count();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2024_day06::part2() {
	int64_t result = 0;

	result = get_guard_loops_obstacles_count();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2024_day06 day06;

	return day06.main_execution();
}
