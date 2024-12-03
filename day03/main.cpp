#include "./../common/aoc.hpp"
#include <regex>

const std::regex C_MUL_TEMPATE("mul\\((\\d{1,3}),(\\d{1,3})\\)");
const std::regex C_DO_TEMPATE("do\\(\\)");
const std::regex C_DONT_TEMPATE("don\\'t\\(\\)");

class AoC2024_day03 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<std::string> programs_;
	uint64_t get_multiplications_result();
	uint64_t get_enabled_multiplications_result();
};

bool AoC2024_day03::init(const std::vector<std::string> lines) {
	programs_ = lines;

	return true;
}

uint64_t AoC2024_day03::get_multiplications_result() {
	uint64_t result = 0;
	std::string tmp;
	std::smatch sm;

	for (size_t i = 0; i < programs_.size(); i++) {
		tmp = programs_[i];

		while (std::regex_search(tmp, sm, C_MUL_TEMPATE)) {
			result += std::stoi(sm.str(1)) * std::stoi(sm.str(2));

			tmp = sm.suffix().str();
		}
	}

	return result;
}

uint64_t AoC2024_day03::get_enabled_multiplications_result() {
	uint64_t result = 0, tmpres;
	std::string tmp, suffm, suffd, suffe;
	std::smatch sm;
	size_t posm, pose, posd;
	bool foundm, founde, foundd, enabled = true;

	for (size_t i = 0; i < programs_.size(); i++) {
		tmp = programs_[i];
		
		while (1) {
			foundm = std::regex_search(tmp, sm, C_MUL_TEMPATE);
			if (foundm) {
				tmpres = std::stoi(sm.str(1)) * std::stoi(sm.str(2));
				posm = sm.position(0);
				suffm = sm.suffix().str();
			} else {
				posm = SIZE_MAX;
			}

			founde = std::regex_search(tmp, sm, C_DO_TEMPATE);
			if (founde) {
				pose = sm.position(0);
				suffe = sm.suffix().str();
			} else {
				pose = SIZE_MAX;
			}

			foundd = std::regex_search(tmp, sm, C_DONT_TEMPATE);
			if (foundd) {
				posd = sm.position(0);
				suffd = sm.suffix().str();
			} else {
				posd = SIZE_MAX;
			}

			if (!foundd && !founde && !foundm) {
				break;
			}

			if ((posm < posd) && (posm < pose)) {
				if (enabled) {
					result += tmpres;
				}
				tmp = suffm;
			} else if ((posd < pose) && (posd < posm)) {
				enabled = false;
				tmp = suffd;
			} else {
				enabled = true;
				tmp = suffe;
			}
		}
	}

	return result;
}

int32_t AoC2024_day03::get_aoc_day() {
	return 3;
}

int32_t AoC2024_day03::get_aoc_year() {
	return 2024;
}

void AoC2024_day03::tests() {
	uint64_t result;

	if (init({"xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))"})) {
		result = get_multiplications_result(); // 161
	}

	if (init({"xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))"})) {
		result = get_enabled_multiplications_result(); // 48
	}
}

bool AoC2024_day03::part1() {
	int64_t result = 0;

	result = get_multiplications_result();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2024_day03::part2() {
	int64_t result = 0;

	result = get_enabled_multiplications_result();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2024_day03 day03;

	return day03.main_execution();
}
