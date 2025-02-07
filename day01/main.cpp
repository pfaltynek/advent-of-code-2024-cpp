#include "./../common/aoc.hpp"
#include <algorithm>
#include <map>
#include <sstream>

class AoC2024_day01 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	uint64_t get_lists_total_distance();
	uint64_t get_lists_similarity_score();
	std::vector<int32_t> list1_, list2_;
};

bool AoC2024_day01::init(const std::vector<std::string> lines) {
	std::stringstream ss;
	std::string tmps;
	int32_t tmpn;

	list1_.clear();
	list2_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		ss.clear();
		ss.str(lines[i]);

		ss >> tmps;
		tmpn = std::stoi(tmps);
		list1_.push_back(tmpn);

		ss >> tmps;
		tmpn = std::stoi(tmps);
		list2_.push_back(tmpn);
	}

	return true;
}

uint64_t AoC2024_day01::get_lists_total_distance() {
	uint64_t result = 0;

	std::sort(list1_.begin(), list1_.end());
	std::sort(list2_.begin(), list2_.end());

	for (size_t i = 0; i < std::min(list1_.size(), list2_.size()); i++) {
		result += std::abs(list1_[i] - list2_[i]);
	}

	return result;
}

uint64_t AoC2024_day01::get_lists_similarity_score() {
	uint64_t result = 0;
	std::map<int32_t, uint32_t> cache = {};
	uint32_t cnt;

	for (size_t i = 0; i < list1_.size(); i++) {
		if (cache.count(list1_[i])) {
			cnt = cache[list1_[i]];
		} else {
			cnt = std::count(list2_.begin(), list2_.end(), list1_[i]);
			cache[list1_[i]] = cnt;
		}

		result += list1_[i] * cnt;
	}

	return result;
}

int32_t AoC2024_day01::get_aoc_day() {
	return 1;
}

int32_t AoC2024_day01::get_aoc_year() {
	return 2024;
}

void AoC2024_day01::tests() {
	uint64_t result;

	if (init({"3   4", "4   3", "2   5", "1   3", "3   9", "3   3"})) {
		result = get_lists_total_distance();   // 11
		result = get_lists_similarity_score(); // 31
	}
}

bool AoC2024_day01::part1() {
	int64_t result = 0;

	result = get_lists_total_distance();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2024_day01::part2() {
	int64_t result = 0;

	result = get_lists_similarity_score();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2024_day01 day01;

	return day01.main_execution();
}
