#include "./../common/aoc.hpp"
#include <algorithm>

struct sector_str {
	bool free_space;
	int64_t id;
	size_t size;
};

class AoC2024_day09 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<sector_str> disc_map_;
	uint64_t get_disc_checksum(const bool part2);
	void compact_disc_part1(std::vector<sector_str>& disc_map);
	void compact_disc_part2(std::vector<sector_str>& disc_map);
};

bool AoC2024_day09::init(const std::vector<std::string> lines) {
	bool is_file = true;
	sector_str sector;
	int64_t id = 0;
	size_t idx = 0;

	disc_map_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (lines[i].find_first_not_of("0123456789") != std::string::npos) {
			std::cout << "Invalid input data" << std::endl;
		}
		for (size_t j = 0; j < lines[i].size(); j++) {
			sector.size = lines[i][j] - '0';
			sector.free_space = !is_file;

			if (is_file) {
				sector.id = id;
				id++;
			} else {
				sector.id = -1;
			}

			is_file = !is_file;

			disc_map_.push_back(sector);
		}
	}

	while (idx < disc_map_.size()) {
		if (disc_map_[idx].size == 0) {
			disc_map_.erase(disc_map_.begin() + idx);
		} else {
			idx++;
		}
	}

	return true;
}

void AoC2024_day09::compact_disc_part1(std::vector<sector_str>& disc_map) {
	size_t left_idx = 0;
	sector_str sector;

	while (left_idx < disc_map.size() - 1) {
		if (disc_map[left_idx].free_space) {
			do {
				sector = disc_map.back();
				disc_map.pop_back();
			} while (sector.free_space);

			if (sector.size < disc_map[left_idx].size) {
				disc_map[left_idx].size -= sector.size;
				disc_map.insert(disc_map.begin() + left_idx, sector);
				left_idx++;
			} else if (sector.size == disc_map[left_idx].size) {
				disc_map[left_idx] = sector;
				left_idx++;
			} else {
				disc_map[left_idx].id = sector.id;
				disc_map[left_idx].free_space = false;
				sector.size -= disc_map[left_idx].size;
				disc_map.push_back(sector);
				left_idx++;
			}
		} else {
			left_idx++;
		}
	}

	left_idx = 0;
	while (left_idx < disc_map.size() - 1) {
		if (disc_map[left_idx].id == disc_map[left_idx + 1].id) {
			disc_map[left_idx].size += disc_map[left_idx + 1].size;
			disc_map.erase(disc_map.begin() + left_idx + 1);
		} else {
			left_idx++;
		}
	}
}

void AoC2024_day09::compact_disc_part2(std::vector<sector_str>& disc_map) {
	size_t left_idx = 0, right_idx = disc_map.size() - 1;
	sector_str sector;
	bool finished = false;

	while (!finished) {
		left_idx = 0;

		while (true) {
			if (right_idx <= 0) {
				finished = true;
				break;
			}

			if (disc_map[right_idx].free_space) {
				right_idx--;
			} else {
				break;
			}
		}

		if (finished) {
			break;
		}

		while (true) {
			if (left_idx >= right_idx) {
				right_idx--;
				break;
			}

			if (!disc_map[left_idx].free_space) {
				left_idx++;
				continue;
			}

			if (disc_map[left_idx].size < disc_map[right_idx].size) {
				left_idx++;
				continue;
			}

			if (disc_map[left_idx].size > disc_map[right_idx].size) {
				sector = disc_map[right_idx];
				disc_map[right_idx].id = -1;
				disc_map[right_idx].free_space = true;
				disc_map[left_idx].size -= sector.size;
				disc_map.insert(disc_map.begin() + left_idx, sector);
				break;
			} else if (disc_map[left_idx].size == disc_map[right_idx].size) {
				std::swap(disc_map[left_idx], disc_map[right_idx]);
				right_idx--;
				break;
			}
		}
	}
}

uint64_t AoC2024_day09::get_disc_checksum(const bool part2) {
	uint64_t result = 0;
	uint64_t pos = 0;
	std::vector<sector_str> disc_map(disc_map_);

	if (part2) {
		compact_disc_part2(disc_map);
	} else {
		compact_disc_part1(disc_map);
	}

	for (size_t i = 0; i < disc_map.size(); i++) {
		if (disc_map[i].free_space) {
			pos += disc_map[i].size;
			continue;
		}

		for (size_t j = 0; j < disc_map[i].size; j++) {
			result += pos * disc_map[i].id;
			pos++;
		}
	}

	return result;
}

int32_t AoC2024_day09::get_aoc_day() {
	return 9;
}

int32_t AoC2024_day09::get_aoc_year() {
	return 2024;
}

void AoC2024_day09::tests() {
	uint64_t result;

	if (init({"2333133121414131402"})) {
		result = get_disc_checksum(false); // 1928
		result = get_disc_checksum(true);  // 2858
	}
}

bool AoC2024_day09::part1() {
	int64_t result = 0;

	result = get_disc_checksum(false);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2024_day09::part2() {
	int64_t result = 0;

	result = get_disc_checksum(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2024_day09 day09;

	return day09.main_execution();
}
