#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <set>

class AoC2024_day08 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::map<coord_str, char> map_;
	std::map<char, std::vector<coord_str>> frequencies_;
	coord_str map_size_;
	bool is_in_map(const coord_str pos);
	uint64_t get_antinode_locations_count(const bool part2);
};

bool AoC2024_day08::init(const std::vector<std::string> lines) {
	size_t size;

	map_.clear();
	frequencies_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {

		if (i) {
			if (lines[i].size() != size) {
				std::cout << "Invalid map line size at line " << i + 1 << std::endl;
				return false;
			}
		} else {
			size = lines[i].size();
		}

		for (uint32_t j = 0; j < lines[i].size(); j++) {
			if (isalnum(lines[i][j])) {
				map_[{j, i}] = lines[i][j];
				frequencies_[lines[i][j]].push_back(coord_str(j, i));
			} else if (lines[i][j] == '.') {
				// ignore
			} else {
				assert(false);
			}
		}
	}
	map_size_ = {size, lines.size()};

	return true;
}

bool AoC2024_day08::is_in_map(const coord_str pos) {
	return ((pos.x >= 0) && (pos.y >= 0) && (pos.x < map_size_.x) && (pos.y < map_size_.y));
}

uint64_t AoC2024_day08::get_antinode_locations_count(const bool part2) {
	std::set<coord_str> result;
	coord_str a1, a2, diff1, diff2;

	result.clear();

	for (auto& freq : frequencies_) {
		for (size_t i = 0; i < freq.second.size(); i++) {
			for (size_t j = i + 1; j < freq.second.size(); j++) {
				diff1 = freq.second[i] - freq.second[j];
				diff2 = freq.second[j] - freq.second[i];

				if (part2) {
					result.emplace(freq.second[j]);
					result.emplace(freq.second[i]);

					a1 = freq.second[j] - diff1;
					while (is_in_map(a1)) {
						result.emplace(a1);
						a1 = a1 - diff1;
					}

					a2 = freq.second[i] - diff2;
					while (is_in_map(a2)) {
						result.emplace(a2);
						a2 = a2 - diff2;
					}
				} else {
					a1 = freq.second[j] - diff1;
					a2 = freq.second[i] - diff2;

					if (is_in_map(a1)) {
						result.emplace(a1);
					}

					if (is_in_map(a2)) {
						result.emplace(a2);
					}
				}
			}
		}
	}

	return result.size();
}

int32_t AoC2024_day08::get_aoc_day() {
	return 8;
}

int32_t AoC2024_day08::get_aoc_year() {
	return 2024;
}

void AoC2024_day08::tests() {
	uint64_t result;

	if (init({"............", "........0...", ".....0......", ".......0....", "....0.......", "......A.....", "............", "............", "........A...",
			  ".........A..", "............", "............"})) {
		result = get_antinode_locations_count(false); // 14
		result = get_antinode_locations_count(true);  // 34
	}
}

bool AoC2024_day08::part1() {
	int64_t result = 0;

	result = get_antinode_locations_count(false);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2024_day08::part2() {
	int64_t result = 0;

	result = get_antinode_locations_count(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2024_day08 day08;

	return day08.main_execution();
}
