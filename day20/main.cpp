#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <queue>
#include <set>

typedef std::pair<coord_str, uint32_t> maze_pos_t;

class AoC2024_day20 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::set<coord_str> walls_;
	std::map<coord_str, uint32_t> steps_map_;
	coord_str size_, start_, end_;
	std::map<coord_str, std::map<coord_str, uint32_t>> cheats_;
	std::map<int64_t, int64_t> stats_;
	void prepare_steps_map();
	void get_all_cheats(const bool part2);
	bool process_cheat(const coord_str step1, const coord_str step2, const uint64_t steps, const uint64_t max_steps);
	int64_t get_cheats_count_saving_at_least_limit(const bool part2, const int64_t limit);
	bool cheat_exists(const coord_str step1, const coord_str step2);
};

bool AoC2024_day20::init(const std::vector<std::string> lines) {
	bool start = false, end = false;

	walls_.clear();

	for (int32_t i = 0; i < static_cast<int32_t>(lines.size()); i++) {
		if (!i) {
			size_ = {lines[i].size(), lines.size()};
		} else {
			if (size_.x != static_cast<int32_t>(lines[i].size())) {
				std::cout << "Invalid map size at line " << i + 1 << std::endl;
				return false;
			}
		}

		if (lines[i].find_first_not_of("#.SE") != std::string::npos) {
			std::cout << "Invalid map content at line " << i + 1 << std::endl;
			return false;
		}

		for (int32_t j = 0; j < static_cast<int32_t>(lines[i].size()); j++) {
			switch (lines[i][j]) {
				case '#':
					walls_.emplace(coord_str(j, i));
					break;
				case '.':
					break;
				case 'S':
					if (!start) {
						start_ = {j, i};
						start = true;
					} else {
						std::cout << "Too much starts found." << std::endl;
						return false;
					}
					break;
				case 'E':
					if (!end) {
						end_ = {j, i};
						end = true;
					} else {
						std::cout << "Too much ends found." << std::endl;
						return false;
					}
					break;
			}
		}
	}

	if (!start) {
		std::cout << "Start not found." << std::endl;
	} else if (!end) {
		std::cout << "End not found." << std::endl;
	} else {
		return true;
	}

	return false;
}

void AoC2024_day20::prepare_steps_map() {
	maze_pos_t current, next;
	std::queue<maze_pos_t> q;
	std::vector<coord_str> directions = {coord_step_north, coord_step_south, coord_step_east, coord_step_west};

	steps_map_.clear();
	q = {};

	current = {end_, 0};
	q.push(current);

	while (!q.empty()) {
		current = q.front();
		q.pop();

		if (walls_.count(current.first)) {
			continue;
		}

		if (steps_map_.count(current.first)) {
			if (steps_map_[current.first] > current.second) {
				steps_map_[current.first] = current.second;
			} else {
				continue;
			}
		} else {
			steps_map_[current.first] = current.second;
		}

		next = current;
		next.second++;

		for (const auto& dir : directions) {
			next.first = current.first + dir;
			q.push(next);
		}
	}
}

bool AoC2024_day20::cheat_exists(const coord_str step1, const coord_str step2) {
	if (cheats_.count(step1)) {
		if (cheats_[step1].count(step2)) {
			return true;
		}
	}

	return false;
}

bool AoC2024_day20::process_cheat(const coord_str step1, const coord_str step2, const uint64_t steps, const uint64_t max_steps) {
	uint64_t steps1, steps2, len, diff;
	bool order = true;

	steps1 = steps_map_[step1];
	steps2 = steps_map_[step2];
	if (steps_map_[step1] < steps_map_[step2]) {
		std::swap(steps1, steps2);
		order = false;
	}

	if (order) {
		if (cheat_exists(step1, step2)) {
			return false;
		}
	} else {
		if (cheat_exists(step2, step1)) {
			return false;
		}
	}

	if (steps1 <= max_steps) {
		len = max_steps + steps2 + steps - steps1;
		if (len < max_steps) {
			diff = max_steps - len;
			if (order) {
				cheats_[step1][step2] = diff;
			} else {
				cheats_[step2][step1] = diff;
			}

			stats_[diff]++;
		}
	}

	return true;
}

void AoC2024_day20::get_all_cheats(const bool part2) {
	std::vector<std::pair<coord_str, coord_str>> cheat_steps = {{coord_step_north, coord_step_south}, {coord_step_east, coord_step_west},
																{coord_step_north, coord_step_east},  {coord_step_north, coord_step_west},
																{coord_step_south, coord_step_east},  {coord_step_south, coord_step_west}};
	std::vector<coord_str> directions = {coord_step_north, coord_step_south, coord_step_east, coord_step_west};
	std::queue<maze_pos_t> q;
	std::set<coord_str> history;
	maze_pos_t current, next;
	uint64_t max_steps;
	coord_str step1, step2;

	if (!part2) {
		prepare_steps_map();
	}

	max_steps = steps_map_[start_];

	cheats_.clear();
	stats_.clear();

	if (part2) {
		for (auto& pos : steps_map_) {
			q = {};
			current.first = pos.first;
			current.second = 0;
			q.push(current);
			history.clear();

			while (!q.empty()) {
				current = q.front();
				q.pop();

				if (current.second > 20) {
					continue;
				}

				if (history.count(current.first)) {
					continue;
				} else {
					history.emplace(current.first);
				}

				if (steps_map_.count(current.first)) {
					if (!process_cheat(pos.first, current.first, current.second, max_steps)) {
						continue;
					}
				}

				next = current;
				next.second++;
				for (auto& dir : directions) {
					next.first = current.first + dir;
					q.push(next);
				}
			}
		}
	} else {
		for (auto& pos : walls_) {
			if ((pos.x < 1) || (pos.y < 1) || (pos.x >= (size_.x - 1)) || (pos.y >= (size_.y - 1))) {
				continue;
			}

			for (auto& steps : cheat_steps) {
				step1 = steps.first + pos;
				step2 = steps.second + pos;

				if ((walls_.count(step1) + walls_.count(step2)) == 0) {
					if (steps_map_.count(step1) + steps_map_.count(step2) == 0) {
						assert(false);
					}

					process_cheat(step1, step2, 2, max_steps);
				}
			}
		}
	}
}

int64_t AoC2024_day20::get_cheats_count_saving_at_least_limit(const bool part2, const int64_t limit) {
	int64_t result = 0;

	get_all_cheats(part2);

	for (auto& pair : stats_) {
		if (pair.first >= limit) {
			result += pair.second;
		}
	}

	return result;
}

int32_t AoC2024_day20::get_aoc_day() {
	return 20;
}

int32_t AoC2024_day20::get_aoc_year() {
	return 2024;
}

void AoC2024_day20::tests() {
	int64_t result;

	if (init({"###############", "#...#...#.....#", "#.#.#.#.#.###.#", "#S#...#.#.#...#", "#######.#.#.###", "#######.#.#...#", "#######.#.###.#",
			  "###..E#...#...#", "###.#######.###", "#...###...#...#", "#.#####.#.###.#", "#.#...#.#.#...#", "#.#.#.#.#.#.###", "#...#...#...###",
			  "###############"})) {
		result = get_cheats_count_saving_at_least_limit(false, 100); // see task description and stats_ content
		result = get_cheats_count_saving_at_least_limit(true, 100);	 // see task description and stats_ content
	}
}

bool AoC2024_day20::part1() {
	int64_t result = 0;

	result = get_cheats_count_saving_at_least_limit(false, 100);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2024_day20::part2() {
	int64_t result;

	result = get_cheats_count_saving_at_least_limit(true, 100);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2024_day20 day20;

	return day20.main_execution();
}
