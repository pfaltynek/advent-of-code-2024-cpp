#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <queue>
#include <set>

class AoC2024_day12 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	coord_str map_size_;
	std::vector<std::string> map_;
	uint64_t get_fencing_total_price(const bool part2);
	uint64_t get_edges_count(const bool horizontal, std::vector<coord_str>& borders);
	bool in_bounds(const coord_str position);
	char get_plant_type(const coord_str position);
};

bool AoC2024_day12::init(const std::vector<std::string> lines) {

	map_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (i) {
			if (lines[i].size() != static_cast<size_t>(map_size_.x)) {
				std::cout << "Invalid map size at line " << i + 1 << std::endl;
				return false;
			}
		} else {
			map_size_.x = lines[i].size();
		}

		map_.push_back(lines[i]);
	}

	map_size_.y = lines.size();

	return true;
}

uint64_t AoC2024_day12::get_edges_count(const bool horizontal, std::vector<coord_str>& borders) {
	int32_t ref;
	std::map<int32_t, std::vector<int32_t>> sorted;
	uint64_t result = 0;

	if (borders.size() <= 1) {
		return borders.size();
	}

	sorted.clear();

	for (auto& coord : borders) {
		if (horizontal) {
			sorted[coord.y].push_back(coord.x);
		} else {
			sorted[coord.x].push_back(coord.y);
		}
	}

	for (auto& pair : sorted) {
		if (pair.second.size() == 1) {
			result++;
		} else {
			std::sort(pair.second.begin(), pair.second.end());

			ref = pair.second[0];

			for (size_t i = 1; i < pair.second.size(); i++) {
				if (ref + 1 != pair.second[i]) {
					result++;
				}

				ref = pair.second[i];
			}
			result++;
		}
	}

	return result;
}

uint64_t AoC2024_day12::get_fencing_total_price(const bool part2) {
	uint64_t result = 0, area, perimeter;
	std::queue<coord_str> q = {}, neighbors = {};
	std::set<coord_str> done = {};
	std::map<coord_str, std::vector<coord_str>> borders;
	coord_str curr_pos, next;
	char curr_plant;
	std::vector<coord_str> directions = {coord_step_north, coord_step_east, coord_step_south, coord_step_west};

	curr_pos = {0, 0};

	neighbors.push(curr_pos);

	while (neighbors.size()) {
		next = neighbors.front();
		neighbors.pop();

		if (done.count(next)) {
			continue;
		}

		q.push(next);
		area = 0;
		perimeter = 0;
		borders.clear();

		while (q.size()) {
			curr_pos = q.front();
			q.pop();

			if (done.count(curr_pos)) {
				continue;
			}

			curr_plant = get_plant_type(curr_pos);
			area++;
			done.emplace(curr_pos);

			for (size_t i = 0; i < directions.size(); i++) {
				next = curr_pos + directions[i];
				if (in_bounds(next)) {
					if (curr_plant == get_plant_type(next)) {
						q.push(next);
					} else {
						neighbors.push(next);
						perimeter++;
						borders[directions[i]].push_back(curr_pos);
					}
				} else {
					perimeter++;
					borders[directions[i]].push_back(curr_pos);
				}
			}
		}

		if (part2) {
			uint64_t edges = 0;

			edges += get_edges_count(true, borders[coord_step_north]);
			edges += get_edges_count(true, borders[coord_step_south]);
			edges += get_edges_count(false, borders[coord_step_east]);
			edges += get_edges_count(false, borders[coord_step_west]);
			result += area * edges;
		} else {
			result += area * perimeter;
		}
	}

	return result;
}

bool AoC2024_day12::in_bounds(const coord_str position) {
	if ((position.x >= 0) && (position.y >= 0) && (position.x < map_size_.x) && (position.y < map_size_.y)) {
		return true;
	}

	return false;
}

char AoC2024_day12::get_plant_type(const coord_str position) {
	return (map_[position.y][position.x]);
}

int32_t AoC2024_day12::get_aoc_day() {
	return 12;
}

int32_t AoC2024_day12::get_aoc_year() {
	return 2024;
}

void AoC2024_day12::tests() {
	uint64_t result;

	if (init({"AAAA", "BBCD", "BBCC", "EEEC"})) {
		result = get_fencing_total_price(false); // 140
		result = get_fencing_total_price(true);	 // 80
	}

	if (init({"OOOOO", "OXOXO", "OOOOO", "OXOXO", "OOOOO"})) {
		result = get_fencing_total_price(false); // 772
		result = get_fencing_total_price(true);	 // 436
	}

	if (init({"AAAAAA", "AAABBA", "AAABBA", "ABBAAA", "ABBAAA", "AAAAAA"})) {
		result = get_fencing_total_price(true); // 368
	}

	if (init({"RRRRIICCFF", "RRRRIICCCF", "VVRRRCCFFF", "VVRCCCJFFF", "VVVVCJJCFE", "VVIVCCJJEE", "VVIIICJJEE", "MIIIIIJJEE", "MIIISIJEEE", "MMMISSJEEE"})) {
		result = get_fencing_total_price(false); // 1930
		result = get_fencing_total_price(true); // 1206
	}
}

bool AoC2024_day12::part1() {
	int64_t result = 0;

	result = get_fencing_total_price(false);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2024_day12::part2() {
	int64_t result = 0;

	result = get_fencing_total_price(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2024_day12 day12;

	return day12.main_execution();
}
