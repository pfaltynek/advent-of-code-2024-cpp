#include "./../common/aoc.hpp"
#include <sstream>

class AoC2024_day07 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<std::vector<uint64_t>> equations_;
	uint64_t get_total_calibration_result(const bool part2);
	bool evaluate_equation(const bool part2, const std::vector<uint64_t>& equation, const size_t idx, uint64_t result);
};

bool AoC2024_day07::init(const std::vector<std::string> lines) {
	std::vector<uint64_t> equation;
	std::stringstream ss;
	uint64_t value;

	equations_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {

		equation.clear();
		ss.clear();
		ss.str(lines[i]);

		ss >> value;
		equation.push_back(value);
		ss.ignore(2);

		while (!ss.eof()) {
			ss >> value;
			equation.push_back(value);
			ss.ignore(1);
		}

		if (equation.size() < 3) {
			std::cout << "Too short equation at line " << i + 1 << std::endl;
			return false;
		}

		equations_.push_back(equation);
	}

	return true;
}

bool AoC2024_day07::evaluate_equation(const bool part2, const std::vector<uint64_t>& equation, const size_t idx, uint64_t result) {
	std::string tmp;
	uint64_t next, x, count;

	if ((result == equation[0]) && (idx == equation.size())) {
		return true;
	}

	if (((idx >= equation.size()) || result > equation[0])) {
		return false;
	}

	if (evaluate_equation(part2, equation, idx + 1, result + equation[idx])) {
		return true;
	}

	if (evaluate_equation(part2, equation, idx + 1, result * equation[idx])) {
		return true;
	}

	if (part2) {
		x = equation[idx];
		next = result;
		while (x > 0) {
			count++;
			x /= 10;
			next *= 10;
		}

		if (evaluate_equation(part2, equation, idx + 1, next + equation[idx])) {
			return true;
		}
	}

	return false;
}

uint64_t AoC2024_day07::get_total_calibration_result(const bool part2) {
	uint64_t result = 0;

	for (size_t i = 0; i < equations_.size(); i++) {
		if (part2) {
			if (evaluate_equation(true, equations_[i], 2, equations_[i][1])) {
				result += equations_[i][0];
			}
		} else {
			if (evaluate_equation(false, equations_[i], 2, equations_[i][1])) {
				result += equations_[i][0];
			}
		}
	}

	return result;
}

int32_t AoC2024_day07::get_aoc_day() {
	return 7;
}

int32_t AoC2024_day07::get_aoc_year() {
	return 2024;
}

void AoC2024_day07::tests() {
	uint64_t result;

	if (init({"190: 10 19", "3267: 81 40 27", "83: 17 5", "156: 15 6", "7290: 6 8 6 15", "161011: 16 10 13", "192: 17 8 14", "21037: 9 7 18 13",
			  "292: 11 6 16 20"})) {
		result = get_total_calibration_result(false); // 3749
		result = get_total_calibration_result(true);  // 11387
	}
}

bool AoC2024_day07::part1() {
	int64_t result = 0;

	result = get_total_calibration_result(false);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2024_day07::part2() {
	int64_t result = 0;

	result = get_total_calibration_result(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2024_day07 day07;

	return day07.main_execution();
}
