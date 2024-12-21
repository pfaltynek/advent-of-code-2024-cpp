#include "./../common/aoc.hpp"
#include <map>
#include <sstream>

class AoC2024_day11 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<uint64_t> stones_;
	uint64_t blink_stones_old(const uint64_t blinks);
	uint64_t blink_stones(const uint64_t blinks);
	uint64_t get_digits_count(uint64_t number);
};

bool AoC2024_day11::init(const std::vector<std::string> lines) {
	std::stringstream ss;
	uint64_t tmp;

	stones_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		ss.clear();
		ss.str(lines[i]);

		while (!ss.eof()) {
			ss >> tmp;
			ss.ignore(1);
			stones_.push_back(tmp);
		}
	}

	return true;
}

uint64_t AoC2024_day11::get_digits_count(uint64_t number) {
	uint64_t result = 0;

	if (!number) {
		return 1;
	}

	while (number) {
		number /= 10;
		result++;
	}

	return result;
}

uint64_t AoC2024_day11::blink_stones_old(const uint64_t blinks) {
	std::vector<uint64_t> result(stones_);
	size_t idx;

	for (size_t i = 0; i < blinks; i++) {

		idx = 0;

		while (idx < result.size()) {
			if (result[idx] == 0) {
				result[idx] = 1;
			} else {
				uint64_t digits = get_digits_count(result[idx]);
				if (digits % 2 == 0) {
					uint64_t n = 1, tmp;

					digits /= 2;

					for (size_t d = 0; d < digits; d++) {
						n *= 10;
					}

					tmp = result[idx];
					result[idx] %= n;
					result.insert(result.begin() + idx, tmp / n);
					idx++;
				} else {
					result[idx] *= 2024;
				}
			}

			idx++;
		}
	}

	return result.size();
}

// better solution with realistic time to finish part 2 - idea from reddit
uint64_t AoC2024_day11::blink_stones(const uint64_t blinks) {
	std::map<uint64_t, uint64_t> map1, map2;
	uint64_t result = 0;

	map1.clear();
	map2.clear();

	for (size_t i = 0; i < stones_.size(); i++) {
		map1[stones_[i]]++;
	}

	for (size_t i = 0; i < blinks; i++) {
		for (auto& pair : map1) {
			if (pair.first == 0) {
				map2[1] = pair.second;
			} else {
				uint64_t digits = get_digits_count(pair.first);
				if (digits % 2 == 0) {
					uint64_t n = 1, tmp;

					digits /= 2;

					for (size_t d = 0; d < digits; d++) {
						n *= 10;
					}

					tmp = pair.first;
					map2[tmp % n] += pair.second;
					map2[tmp / n] += pair.second;
				} else {
					map2[pair.first * 2024] += pair.second;
				}
			}
		}
		map1.swap(map2);
		map2.clear();
	}

	for (auto& pair : map1) {
		result += pair.second;
	}
	
	return result;
}

int32_t AoC2024_day11::get_aoc_day() {
	return 11;
}

int32_t AoC2024_day11::get_aoc_year() {
	return 2024;
}

void AoC2024_day11::tests() {
	uint64_t result;

	if (init({"125 17"})) {
		result = blink_stones(25); // 55312
	}
}

bool AoC2024_day11::part1() {
	int64_t result = 0;

	result = blink_stones(25);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2024_day11::part2() {
	int64_t result = 0;

	result = blink_stones(75);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2024_day11 day11;

	return day11.main_execution();
}
