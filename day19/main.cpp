#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
#include <map>
#include <queue>

// My original solution was too slow until i found suggestionn on Reddit to use caching
//
// Maybe speed was also increased by limiting the complete set of patterns to subset of those
// found anywhere in the processed design and also use only soubset of patterns stating with same
// letter as the processed design

class AoC2024_day19 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<std::string> patterns_, designs_;
	std::map<char, std::vector<std::string>> p_;
	std::map<std::string, bool> cache1_;
	std::map<std::string, uint64_t> cache2_;
	uint64_t get_possible_designs_count();
	uint64_t get_all_possible_designs_ways_count();
	uint64_t get_all_possible_design_ways_count(std::string desing);
	bool is_design_possible(std::string design);
};

bool AoC2024_day19::init(const std::vector<std::string> lines) {
	std::string input = "";
	bool patterns = true;

	patterns_.clear();
	designs_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (patterns) {
			if (lines[i].empty()) {
				patterns_ = split(input, ", ");
				patterns = false;
			} else {
				input.append(lines[i]);
			}
		} else {
			designs_.push_back(lines[i]);
		}
	}

	return true;
}

bool AoC2024_day19::is_design_possible(std::string design) {
	size_t len;
	std::string subdesign;

	if (design.empty()) {
		return true;
	}

	if (cache1_.count(design)) {
		return cache1_[design];
	}

	for (auto pattern : p_[design[0]]) {
		len = pattern.size();

		if (design.compare(0, len, pattern) == 0) {
			subdesign = design.substr(len);

			if (is_design_possible(subdesign)) {
				cache1_[design] = true;
				return true;
			} else {
				cache1_[subdesign] = false;
			}
		}
	}

	return false;
}

uint64_t AoC2024_day19::get_possible_designs_count() {
	uint64_t result = 0;

	cache1_.clear();

	for (auto design : designs_) {
		p_.clear();

		for (auto patt : patterns_) {
			if (design.find(patt) != std::string::npos) {
				p_[patt[0]].push_back(patt);
			}
		}

		if (is_design_possible(design)) {
			result++;
		}
	}
	return result;
}

uint64_t AoC2024_day19::get_all_possible_design_ways_count(std::string design) {
	uint64_t result = 0;
	size_t len;
	std::string subdesign;

	if (design.empty()) {
		return 1;
	}

	if (cache2_.count(design)) {
		return cache2_[design];
	}

	for (auto pattern : p_[design[0]]) {
		len = pattern.size();

		if (design.compare(0, len, pattern) == 0) {
			subdesign = design.substr(len);
			result += get_all_possible_design_ways_count(subdesign);
		}
	}

	cache2_[design] = result;

	return result;
}

uint64_t AoC2024_day19::get_all_possible_designs_ways_count() {
	uint64_t result = 0;

	cache2_.clear();

	for (auto design : designs_) {

		if (!cache1_[design]) {
			continue;
		}

		p_.clear();

		for (auto patt : patterns_) {
			if (design.find(patt) != std::string::npos) {
				p_[patt[0]].push_back(patt);
			}
		}

		result += get_all_possible_design_ways_count(design);
	}
	return result;
}

int32_t AoC2024_day19::get_aoc_day() {
	return 19;
}

int32_t AoC2024_day19::get_aoc_year() {
	return 2024;
}

void AoC2024_day19::tests() {
	int64_t result;

	if (init({"r, wr, b, g, bwu, rb, gb, br", "", "brwrr", "bggr", "gbbr", "rrbgbr", "ubwu", "bwurrg", "brgr", "bbrgwb"})) {
		result = get_possible_designs_count(); // 6
		result = get_all_possible_designs_ways_count(); // 16
	}
}

bool AoC2024_day19::part1() {
	int64_t result = 0;

	result = get_possible_designs_count();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2024_day19::part2() {
	int64_t result;

	result = get_all_possible_designs_ways_count();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2024_day19 day19;

	return day19.main_execution();
}
