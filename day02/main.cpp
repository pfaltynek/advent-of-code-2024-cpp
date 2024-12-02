#include "./../common/aoc.hpp"
#include <sstream>

class AoC2024_day02 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	uint64_t get_safe_reports_count(const bool part2);
	std::vector<std::vector<uint32_t>> reports_;
	bool is_safe(uint32_t num1, uint32_t num2, bool& increasing);
	bool is_report_safe(std::vector<uint32_t>& report);
};

bool AoC2024_day02::init(const std::vector<std::string> lines) {
	std::stringstream ss;
	std::string tmps;
	int32_t tmpn;
	std::vector<uint32_t> report;

	reports_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		ss.clear();
		ss.str(lines[i]);

		report.clear();

		while (!ss.eof()) {
			ss >> tmps;
			tmpn = std::stoi(tmps);
			report.push_back(tmpn);
		}

		reports_.push_back(report);
	}

	return true;
}

bool AoC2024_day02::is_safe(uint32_t num1, uint32_t num2, bool& increasing) {
	increasing = true;
	uint32_t diff;

	if (num1 == num2) {
		return false;
	} else if (num1 > num2) {
		diff = num1 - num2;
		increasing = false;
	} else {
		diff = num2 - num1;
	}

	if (diff <= 3) {
		return true;
	} else {
		return false;
	}
}

bool AoC2024_day02::is_report_safe(std::vector<uint32_t>& report) {
	bool inc, tmp, safe = true;

	for (size_t i = 0; i < report.size() - 1; i++) {
		if (is_safe(report[i], report[i + 1], tmp)) {
			if (i == 0) {
				inc = tmp;
			} else {
				if (tmp != inc) {
					safe = false;
					break;
				}
			}
		} else {
			safe = false;
			break;
		}
	}

	return safe;
}

uint64_t AoC2024_day02::get_safe_reports_count(const bool part2) {
	uint64_t result = 0;
	std::vector<uint32_t> tmp;

	for (size_t i = 0; i < reports_.size(); i++) {
		if (is_report_safe(reports_[i])) {
			result++;
		} else {
			if (part2) {
				for (size_t j = 0; j < reports_[i].size(); j++)
				{
					tmp = reports_[i];
					tmp.erase(tmp.begin() + j);

					if (is_report_safe(tmp)) {
						result++;
						break;
					}
				}
				
			}
		}
	}

	return result;
}

int32_t AoC2024_day02::get_aoc_day() {
	return 2;
}

int32_t AoC2024_day02::get_aoc_year() {
	return 2024;
}

void AoC2024_day02::tests() {
	uint64_t result;

	if (init({"7 6 4 2 1", "1 2 7 8 9", "9 7 6 2 1", "1 3 2 4 5", "8 6 4 4 1", "1 3 6 7 9"})) {
		result = get_safe_reports_count(false); // 2
		result = get_safe_reports_count(true);	// 4
	}
}

bool AoC2024_day02::part1() {
	int64_t result = 0;

	result = get_safe_reports_count(false);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2024_day02::part2() {
	int64_t result = 0;

	result = get_safe_reports_count(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2024_day02 day02;

	return day02.main_execution();
}
