#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>

const std::string C_WORD_TO_SEARCH = "XMAS";

class AoC2024_day04 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::map<coord_str, char> crossword_;
	uint64_t get_xmas_word_appearance_count();
	uint64_t get_x_mas_word_appearance_count();
};

bool AoC2024_day04::init(const std::vector<std::string> lines) {
	crossword_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		for (uint32_t j = 0; j < lines[i].size(); j++) {
			crossword_[{j, i}] = lines[i][j];
		}
	}

	return true;
}

uint64_t AoC2024_day04::get_xmas_word_appearance_count() {
	uint64_t result = 0;
	std::vector<coord_str> directions = {coord_step_north,		coord_step_south,	   coord_step_west,		  coord_step_east,
										 coord_step_north_east, coord_step_south_east, coord_step_north_west, coord_step_south_west};
	coord_str tmp;

	for (auto& pair : crossword_) {
		if (pair.second == C_WORD_TO_SEARCH[0]) {
			for (size_t i = 0; i < directions.size(); i++) {
				bool found = true;
				tmp = pair.first;

				for (size_t j = 1; j < C_WORD_TO_SEARCH.size(); j++) {
					tmp = tmp + directions[i];
					if (crossword_[tmp] != C_WORD_TO_SEARCH[j]) {
						found = false;
						break;
					}
				}

				if (found) {
					result++;
				}
			}
		}
	}

	return result;
}

uint64_t AoC2024_day04::get_x_mas_word_appearance_count() {
	uint64_t result = 0;
	coord_str tmp;

	for (auto& pair : crossword_) {
		if (pair.second == C_WORD_TO_SEARCH[2]) {
			tmp = pair.first;

			if ((((crossword_[tmp + coord_step_north_east] == C_WORD_TO_SEARCH[1]) && (crossword_[tmp + coord_step_south_west] == C_WORD_TO_SEARCH[3])) ||
				 ((crossword_[tmp + coord_step_north_east] == C_WORD_TO_SEARCH[3]) && (crossword_[tmp + coord_step_south_west] == C_WORD_TO_SEARCH[1]))) &&
				(((crossword_[tmp + coord_step_south_east] == C_WORD_TO_SEARCH[1]) && (crossword_[tmp + coord_step_north_west] == C_WORD_TO_SEARCH[3])) ||
				 ((crossword_[tmp + coord_step_south_east] == C_WORD_TO_SEARCH[3]) && (crossword_[tmp + coord_step_north_west] == C_WORD_TO_SEARCH[1])))) {
				result++;
			}
		}
	}

	return result;
}

int32_t AoC2024_day04::get_aoc_day() {
	return 4;
}

int32_t AoC2024_day04::get_aoc_year() {
	return 2024;
}

void AoC2024_day04::tests() {
	uint64_t result;

	if (init({"..X...", ".SAMX.", ".A..A.", "XMAS.S", ".X...."})) {
		result = get_xmas_word_appearance_count(); // 4
	}

	if (init({"MMMSXXMASM", "MSAMXMSMSA", "AMXSXMAAMM", "MSAMASMSMX", "XMASAMXAMM", "XXAMMXXAMA", "SMSMSASXSS", "SAXAMASAAA", "MAMMMXMMMM", "MXMXAXMASX"})) {
		result = get_xmas_word_appearance_count(); // 18
	}

	if (init({"M.S", ".A.", "M.S"})) {
		result = get_x_mas_word_appearance_count(); // 1
	}

	if (init({".M.S......", "..A..MSMS.", ".M.S.MAA..", "..A.ASMSM.", ".M.S.M....", "..........", "S.S.S.S.S.", ".A.A.A.A..", "M.M.M.M.M.", ".........."})) {
		result = get_x_mas_word_appearance_count(); // 9
	}
}

bool AoC2024_day04::part1() {
	int64_t result = 0;

	result = get_xmas_word_appearance_count();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2024_day04::part2() {
	int64_t result = 0;

	result = get_x_mas_word_appearance_count();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2024_day04 day04;

	return day04.main_execution();
}
