#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <queue>
#include <set>

struct maze_position_str {
	uint32_t points;
	coord_str position;
	coord_str direction;
	std::set<coord_str> history;
};

class AoC2024_day16 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::set<coord_str> walls_;
	coord_str start_, end_, map_size_;
	std::map<coord_str, std::map<coord_str, uint32_t>> score_map_;
	uint64_t get_lowest_score();
	uint64_t get_best_paths_tiles_count();
};

bool AoC2024_day16::init(const std::vector<std::string> lines) {
	bool start_found = false, end_found = false;

	walls_.clear();

	map_size_.y = lines.size();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (!i) {
			map_size_.x = lines[i].size();
		} else if (lines[i].size() != static_cast<size_t>(map_size_.x)) {
			std::cout << "Invalid map size at line " << i + 1 << std::endl;
			return false;
		}

		if (lines[i].find_first_not_of("#.SE") != std::string::npos) {
			std::cout << "Unknown map symbol at line " << i + 1 << std::endl;
			return false;
		}

		for (uint32_t j = 0; j < lines[i].size(); j++) {
			switch (lines[i][j]) {
				case '#':
					walls_.emplace(coord_str(j, i));
					break;
				case '.':
					break;
				case 'S':
					start_ = coord_str(j, i);
					start_found = true;
					break;
				case 'E':
					end_ = coord_str(j, i);
					end_found = true;
					break;
			}
		}
	}

	if (!start_found) {
		std::cout << "Map start position not found." << std::endl;
	} else if (!end_found) {
		std::cout << "Map end position not found." << std::endl;
	} else {
		return true;
	}

	return false;
}

uint64_t AoC2024_day16::get_lowest_score() {
	uint64_t result = UINT64_MAX;
	std::queue<maze_position_str> q;
	maze_position_str pos, next;

	score_map_.clear();

	pos.position = start_;
	pos.direction = coord_step_east;
	pos.points = 0;

	q.push(pos);

	while (q.size()) {
		pos = q.front();
		q.pop();

		if (walls_.count(pos.position)) {
			continue;
		}

		if (score_map_.count(pos.position)) {
			if (score_map_[pos.position].count(pos.direction)) {
				if (score_map_[pos.position][pos.direction] > pos.points) {
					score_map_[pos.position][pos.direction] = pos.points;
				} else {
					continue;
				}
			} else {
				score_map_[pos.position][pos.direction] = pos.points;
			}
		} else {
			score_map_[pos.position][pos.direction] = pos.points;
		}

		next = pos;
		next.position = pos.position + pos.direction;

		next.points++;
		q.push(next);

		next = pos;
		next.points += 1000;

		if ((pos.direction == coord_step_east) || (pos.direction == coord_step_west)) {
			next.direction = coord_step_north;
			q.push(next);
			next.direction = coord_step_south;
			q.push(next);
		} else if ((pos.direction == coord_step_north) || (pos.direction == coord_step_south)) {
			next.direction = coord_step_east;
			q.push(next);
			next.direction = coord_step_west;
			q.push(next);
		} else {
			assert(false);
			return 0;
		}
	}

	for (auto& pair : score_map_[end_]) {
		if (pair.second < result) {
			result = pair.second;
		}
	}

	return result;
}

uint64_t AoC2024_day16::get_best_paths_tiles_count() {
	std::set<coord_str> result;
	std::queue<maze_position_str> q;
	maze_position_str pos, next;
	std::vector<coord_str> neighbors = {coord_step_north, coord_step_south, coord_step_east, coord_step_west};
	coord_str tmp;
	uint32_t pts;

	result.clear();

	pos.position = end_;

	pts = UINT32_MAX;
	for (auto& pair : score_map_[end_]) {
		if (pair.second < pts) {
			pts = pair.second;
		}
	}

	pos.points = pts;

	for (auto& pair : score_map_[end_]) {
		if (pair.second == pts) {
			pos.direction = pair.first;
			q.push(pos);
		}
	}

	result.emplace(end_);

	while (q.size()) {
		pos = q.front();
		q.pop();

		if (pos.position == start_) {
			result.insert(pos.history.begin(), pos.history.end());
			continue;
		}

		if (walls_.count(pos.position)) {
			continue;
		}

		if (score_map_.count(pos.position)) {
			if (score_map_[pos.position].count(pos.direction)) {
				if (score_map_[pos.position][pos.direction] == pos.points) {
					next = pos;
					next.position = next.position - pos.direction;
					next.points--;
					next.history.emplace(next.position);
					q.push(next);
				}
			}

			next = pos;
			next.points -= 1000;

			if ((pos.direction == coord_step_east) || (pos.direction == coord_step_west)) {
				next.direction = coord_step_north;
				if (score_map_[next.position][next.direction] == next.points) {
					q.push(next);
				}
				next.direction = coord_step_south;
				if (score_map_[next.position][next.direction] == next.points) {
					q.push(next);
				}
			} else if ((pos.direction == coord_step_north) || (pos.direction == coord_step_south)) {
				next.direction = coord_step_east;
				if (score_map_[next.position][next.direction] == next.points) {
					q.push(next);
				}
				next.direction = coord_step_west;
				if (score_map_[next.position][next.direction] == next.points) {
					q.push(next);
				}
			} else {
				assert(false);
				return 0;
			}

		}
	}

	return result.size();
}

int32_t AoC2024_day16::get_aoc_day() {
	return 16;
}

int32_t AoC2024_day16::get_aoc_year() {
	return 2024;
}

void AoC2024_day16::tests() {
	uint64_t result;

	if (init({"###############", "#.......#....E#", "#.#.###.#.###.#", "#.....#.#...#.#", "#.###.#####.#.#", "#.#.#.......#.#", "#.#.#####.###.#",
			  "#...........#.#", "###.#.#####.#.#", "#...#.....#.#.#", "#.#.#.###.#.#.#", "#.....#...#.#.#", "#.###.#.#.#.#.#", "#S..#.....#...#",
			  "###############"})) {
		result = get_lowest_score();		   // 7036
		result = get_best_paths_tiles_count(); // 45
	}

	if (init({"#################", "#...#...#...#..E#", "#.#.#.#.#.#.#.#.#", "#.#.#.#...#...#.#", "#.#.#.#.###.#.#.#", "#...#.#.#.....#.#", "#.#.#.#.#.#####.#",
			  "#.#...#.#.#.....#", "#.#.#####.#.###.#", "#.#.#.......#...#", "#.#.###.#####.###", "#.#.#...#.....#.#", "#.#.#.#####.###.#", "#.#.#.........#.#",
			  "#.#.#.#########.#", "#S#.............#", "#################"})) {
		result = get_lowest_score();		   // 11048
		result = get_best_paths_tiles_count(); // 64
	}
}

bool AoC2024_day16::part1() {
	int64_t result = 0;

	result = get_lowest_score();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2024_day16::part2() {
	int64_t result = 0;

	result = get_best_paths_tiles_count();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2024_day16 day16;

	return day16.main_execution();
}
