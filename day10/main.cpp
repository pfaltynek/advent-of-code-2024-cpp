#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <queue>
#include <set>

struct step_str {
	coord_str position;
	std::string steps;

	void update(coord_str point) {
		position = point;
		steps += '|' + std::to_string(point.x) + '|' + std::to_string(point.y) + '|';
	}
};

class AoC2024_day10 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<std::string> map_;
	coord_str map_size_;
	uint64_t get_trailhead_scores_sum(const bool part2);
	uint64_t get_trailhead_score(const coord_str trailhead);
	uint64_t get_trailhead_rating(const coord_str trailhead);
	bool in_bounds(const coord_str position);
};

bool AoC2024_day10::init(const std::vector<std::string> lines) {

	map_.clear();

	map_size_.y = lines.size();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (lines[i].find_first_not_of("0123456789") != std::string::npos) {
			std::cout << "Invalid input data at line " << i + 1 << std::endl;
			return false;
		}
		if (i) {
			if (lines[i].size() != static_cast<size_t>(map_size_.x)) {
				std::cout << "Invalid input map size at line " << i + 1 << std::endl;
				return false;
			}
		} else {
			map_size_.x = lines[i].size();
		}

		map_.push_back(lines[i]);
	}

	return true;
}

bool AoC2024_day10::in_bounds(const coord_str position) {
	if ((position.x < map_size_.x) && (position.y < map_size_.y) && (position.x >= 0) && (position.y >= 0)) {
		return true;
	}

	return false;
}

uint64_t AoC2024_day10::get_trailhead_score(const coord_str trailhead) {
	std::set<coord_str> result = {};
	std::queue<coord_str> q = {};
	std::vector<coord_str> directions = {coord_step_north, coord_step_east, coord_step_south, coord_step_west};
	coord_str curr, tmp;
	char next;

	q.push(trailhead);

	while (q.size()) {
		curr = q.front();
		q.pop();

		next = map_[curr.y][curr.x] + 1;

		for (auto& direction : directions) {
			tmp = curr + direction;

			if (in_bounds(tmp)) {
				if (map_[tmp.y][tmp.x] == next) {
					if (next == '9') {
						result.emplace(tmp);
					} else {
						q.push(tmp);
					}
				}
			}
		}
	}

	return result.size();
}

uint64_t AoC2024_day10::get_trailhead_rating(const coord_str trailhead) {
	std::set<std::string> result = {};
	std::queue<step_str> q = {};
	std::vector<coord_str> directions = {coord_step_north, coord_step_east, coord_step_south, coord_step_west};
	step_str curr, tmp;
	char next;
	coord_str pos;

	q.push({
		trailhead,
	});

	while (q.size()) {
		curr = q.front();
		q.pop();

		next = map_[curr.position.y][curr.position.x] + 1;

		for (auto& direction : directions) {
			tmp = curr;
			pos = tmp.position + direction;

			if (in_bounds(pos)) {
				tmp.update(pos);

				if (map_[tmp.position.y][tmp.position.x] == next) {
					if (next == '9') {
						result.emplace(tmp.steps);
					} else {
						q.push(tmp);
					}
				}
			}
		}
	}

	return result.size();
}

uint64_t AoC2024_day10::get_trailhead_scores_sum(const bool part2) {
	uint64_t result = 0;
	std::vector<coord_str> trailheads = {};
	size_t pos;

	for (size_t i = 0; i < map_.size(); i++) {
		pos = 0;
		while (1) {
			pos = map_[i].find('0', pos);

			if (pos != std::string::npos) {
				trailheads.push_back({pos, i});
				pos++;
			} else {
				break;
			}
		}
	}

	for (auto& position : trailheads) {
		if (part2) {
			result += get_trailhead_rating(position);
		} else {
			result += get_trailhead_score(position);
		}
	}

	return result;
}

int32_t AoC2024_day10::get_aoc_day() {
	return 10;
}

int32_t AoC2024_day10::get_aoc_year() {
	return 2024;
}

void AoC2024_day10::tests() {
	uint64_t result;

	if (init({"89010123", "78121874", "87430965", "96549874", "45678903", "32019012", "01329801", "10456732"})) {
		result = get_trailhead_scores_sum(false); // 36
		result = get_trailhead_scores_sum(true);  // 81
	}
}

bool AoC2024_day10::part1() {
	int64_t result = 0;

	result = get_trailhead_scores_sum(false);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2024_day10::part2() {
	int64_t result = 0;

	result = get_trailhead_scores_sum(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2024_day10 day10;

	return day10.main_execution();
}
