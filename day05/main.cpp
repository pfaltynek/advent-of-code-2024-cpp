#include "./../common/aoc.hpp"
#include <map>
#include <sstream>

const std::string C_WORD_TO_SEARCH = "XMAS";

class AoC2024_day05 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::map<uint16_t, std::vector<uint16_t>> rules_;
	std::vector<std::vector<uint16_t>> updates_;
	std::vector<size_t> incorrectly_ordered_updates_;
	uint64_t get_correctly_ordered_updates_middle_page_sum();
	uint64_t get_corrected_updates_ordering_middle_page_sum();
};

bool AoC2024_day05::init(const std::vector<std::string> lines) {
	bool rules = true;
	std::stringstream ss;
	uint16_t rule1, rule2, tmp;
	std::vector<uint16_t> update;

	rules_.clear();
	updates_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {

		if (lines[i].empty()) {
			rules = false;
			continue;
		}

		ss.clear();
		ss.str(lines[i]);

		if (rules) {
			ss >> rule1;
			ss.ignore(1);
			ss >> rule2;

			rules_[rule1].push_back(rule2);
		} else {
			update.clear();

			while (!ss.eof()) {
				ss >> tmp;
				update.push_back(tmp);
				ss.ignore(1);
			}

			updates_.push_back(update);
		}
	}

	return true;
}

uint64_t AoC2024_day05::get_correctly_ordered_updates_middle_page_sum() {
	uint64_t result = 0;
	std::map<uint16_t, uint16_t> positions;
	bool correct;

	incorrectly_ordered_updates_.clear();

	for (size_t i = 0; i < updates_.size(); i++) {
		positions.clear();
		correct = true;

		for (size_t j = 0; j < updates_[i].size(); j++) {
			positions[updates_[i][j]] = j;
		}

		for (size_t j = 0; j < updates_[i].size(); j++) {
			if (rules_.count(updates_[i][j])) {
				for (size_t k = 0; k < rules_[updates_[i][j]].size(); k++) {
					if (positions.count(rules_[updates_[i][j]][k])) {
						if (positions[rules_[updates_[i][j]][k]] < positions[updates_[i][j]]) {
							correct = false;
							break;
						}
					}
				}

				if (!correct) {
					break;
				}
			}
		}

		if (correct) {
			result += updates_[i][updates_[i].size() / 2];
		} else {
			incorrectly_ordered_updates_.push_back(i);
		}
	}

	return result;
}

uint64_t AoC2024_day05::get_corrected_updates_ordering_middle_page_sum() {
	uint64_t result = 0;
	std::map<uint16_t, uint16_t> positions;
	std::vector<uint16_t> tmp;
	uint16_t value;
	size_t new_pos;
	bool correct;

	for (size_t i = 0; i < incorrectly_ordered_updates_.size(); i++) {
		tmp = updates_[incorrectly_ordered_updates_[i]];

		correct = false;

		while (!correct) {
			correct = true;
			positions.clear();

			for (size_t j = 0; j < tmp.size(); j++) {
				positions[tmp[j]] = j;
			}

			for (size_t j = 0; j < tmp.size(); j++) {
				if (rules_.count(tmp[j])) {
					for (size_t k = 0; k < rules_[tmp[j]].size(); k++) {
						if (positions.count(rules_[tmp[j]][k])) {
							if (positions[rules_[tmp[j]][k]] < positions[tmp[j]]) {
								value = tmp[j];
								new_pos = positions[rules_[tmp[j]][k]];
								tmp.erase(tmp.begin() + j);
								tmp.insert(tmp.begin() + new_pos, value);
								correct = false;
								break;
							}
						}
					}

					if (!correct) {
						break;
					}
				}
			}
		}

		result += tmp[tmp.size() / 2];
	}

	return result;
}

int32_t AoC2024_day05::get_aoc_day() {
	return 5;
}

int32_t AoC2024_day05::get_aoc_year() {
	return 2024;
}

void AoC2024_day05::tests() {
	uint64_t result;

	if (init({"47|53", "97|13", "97|61",		  "97|47",			"75|29",	"61|13",		  "75|53",	  "29|13",		   "97|29", "53|29",
			  "61|53", "97|53", "61|29",		  "47|13",			"75|47",	"97|75",		  "47|61",	  "75|61",		   "47|29", "75|13",
			  "53|13", "",		"75,47,61,53,29", "97,61,53,29,13", "75,29,13", "75,97,47,61,53", "61,13,29", "97,13,75,29,47"})) {
		result = get_correctly_ordered_updates_middle_page_sum();  // 143
		result = get_corrected_updates_ordering_middle_page_sum(); // 123
	}
}

bool AoC2024_day05::part1() {
	int64_t result = 0;

	result = get_correctly_ordered_updates_middle_page_sum();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2024_day05::part2() {
	int64_t result = 0;

	result = get_corrected_updates_ordering_middle_page_sum();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2024_day05 day05;

	return day05.main_execution();
}
