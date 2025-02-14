#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <queue>
#include <set>
#include <sstream>

struct memory_step_str {
	coord_str position;
	uint64_t steps;
};

class AoC2024_day18 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<coord_str> bytes_;
	uint64_t get_steps_count(const int32_t size, const uint32_t bytes_count);
	coord_str find_path_cut_off_byte_position(const int32_t size, const uint32_t bytes_count);
};

bool AoC2024_day18::init(const std::vector<std::string> lines) {
	std::stringstream ss;
	int32_t x, y;

	bytes_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		ss.clear();
		ss.str(lines[i]);
		ss >> x;
		ss.ignore(1);
		ss >> y;

		bytes_.push_back({x, y});
	}

	return true;
}

uint64_t AoC2024_day18::get_steps_count(const int32_t size, const uint32_t bytes_count) {
	uint64_t result = 0;
	std::set<coord_str> corrupted = {}, history = {};
	std::queue<memory_step_str> q;
	coord_str end;
	memory_step_str curr, next;
	std::vector<coord_str> directions = {coord_step_north, coord_step_south, coord_step_east, coord_step_west};

	end = {size, size};
	curr.position = {0, 0};
	curr.steps = 0;
	q.push(curr);

	corrupted.insert(bytes_.begin(), bytes_.begin() + bytes_count);

	while (q.size()) {
		curr = q.front();
		q.pop();

		if (curr.position == end) {
			result = curr.steps;
			break;
		}

		if ((curr.position.x < 0) || (curr.position.y < 0) || (curr.position.x > size) || (curr.position.y > size)) {
			continue;
		}

		if (history.count(curr.position)) {
			continue;
		}

		if (corrupted.count(curr.position)) {
			continue;
		}

		history.emplace(curr.position);

		next = curr;
		next.steps++;

		for (auto direction : directions) {
			next.position = curr.position + direction;
			q.push(next);
		}
	}

	return result;
}

coord_str AoC2024_day18::find_path_cut_off_byte_position(const int32_t size, const uint32_t bytes_count) {
	uint32_t min, max, curr;
	uint64_t steps;
	std::map<uint32_t, uint64_t> cache;
	min = bytes_count;
	max = bytes_.size();
	coord_str result = {0, 0};

	while (true) {
		curr = min + ((max - min) / 2);

		steps = get_steps_count(size, curr);

		if (steps) {
			min = curr;
		} else {
			max = curr;
		}

		if ((max - min) == 1) {
			steps = get_steps_count(size, min);
			if (!steps) {
				result = bytes_[min - 1];
			} else {
				steps = get_steps_count(size, max);
				if (!steps) {
					result = bytes_[max - 1];
				}
			}

			break;
		}
	}

	return result;
}

int32_t AoC2024_day18::get_aoc_day() {
	return 18;
}

int32_t AoC2024_day18::get_aoc_year() {
	return 2024;
}

void AoC2024_day18::tests() {
	std::string result;
	coord_str coord;

	if (init({"5,4", "4,2", "4,5", "3,0", "2,1", "6,3", "2,4", "1,5", "0,6", "3,3", "2,6", "5,1", "1,2",
			  "5,5", "2,5", "6,5", "1,4", "0,4", "6,4", "1,1", "6,1", "1,0", "0,5", "1,6", "2,0"})) {
		result = get_steps_count(6, 12);				// 22
		coord = find_path_cut_off_byte_position(6, 12); // 6,1
	}
}

bool AoC2024_day18::part1() {
	int64_t result = 0;

	result = get_steps_count(70, 1024);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2024_day18::part2() {
	coord_str result;

	result = find_path_cut_off_byte_position(70, 1024);

	result2_ = std::to_string(result.x) + "," + std::to_string(result.y);

	return true;
}

int main(void) {
	AoC2024_day18 day18;

	return day18.main_execution();
}
