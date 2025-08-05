#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <cstring>
#include <sstream>

struct robot_str {
	coord_str position;
	coord_str velocity;
};

const int64_t C_WIDTH_TEST = 11;
const int64_t C_HEIGHT_TEST = 7;
const int64_t C_WIDTH_PART = 101;
const int64_t C_HEIGHT_PART = 103;

class AoC2024_day14 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<robot_str> robots_;
	uint64_t get_safety_factor(const int64_t steps, const int64_t width, const int64_t height);
	uint64_t get_xmass_tree(const int64_t steps, const int64_t width, const int64_t height);
};

bool AoC2024_day14::init(const std::vector<std::string> lines) {
	std::stringstream ss;
	robot_str robot;

	robots_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		ss.clear();
		ss.str(lines[i]);

		ss.ignore(2); // "p="
		ss >> robot.position.x;
		ss.ignore(1); // ","
		ss >> robot.position.y;
		ss.ignore(3); // " v="
		ss >> robot.velocity.x;
		ss.ignore(1); // ","
		ss >> robot.velocity.y;

		robots_.push_back(robot);
	}

	return true;
}

uint64_t AoC2024_day14::get_safety_factor(const int64_t steps, const int64_t width, const int64_t height) {
	uint64_t q1 = 0, q2 = 0, q3 = 0, q4 = 0;
	int32_t qw, qh;
	coord_str pos;

	qw = width / 2;
	qh = height / 2;

	for (size_t i = 0; i < robots_.size(); i++) {
		pos = robots_[i].position;
		pos.x = (((pos.x + (steps * robots_[i].velocity.x) % width)) + width) % width;
		pos.y = (((pos.y + (steps * robots_[i].velocity.y)) % height) + height) % height;

		if (pos.x < qw) {
			if (pos.y < qh) {
				q1++;
			} else if (pos.y > qh) {
				q3++;
			}
		} else if (pos.x > qw) {
			if (pos.y < qh) {
				q2++;
			} else if (pos.y > qh) {
				q4++;
			}
		}
	}

	return q1 * q2 * q3 * q4;
}

uint64_t AoC2024_day14::get_xmass_tree(const int64_t steps, const int64_t width, const int64_t height) {
	coord_str pos;
	char map[width][height];
	uint8_t cnt;
	bool print;

	for (int64_t x = 0; x < INT64_MAX; x++) {
		memset(map, '.', width * height);

		for (size_t i = 0; i < robots_.size(); i++) {
			pos = robots_[i].position;
			pos.x = (((pos.x + (x * robots_[i].velocity.x) % width)) + width) % width;
			pos.y = (((pos.y + (x * robots_[i].velocity.y)) % height) + height) % height;

			map[pos.x][pos.y] = '*';
		}

		for (int64_t j = 0; j < height; j++) {
			cnt = 0;
			for (int64_t i = 0; i < width; i++) {
				if (map[i][j] == '*') {
					cnt++;
				} else {
					cnt = 0;
				}

				if (cnt == 8) {
					print = true;
					cnt = 0;
				}
			}
		}

		if (print) {
			for (int64_t j = 0; j < height; j++) {
				for (int64_t i = 0; i < width; i++) {
					std::cout << map[i][j];
				}
				std::cout << std::endl;
			}

			std::cout << std::endl;
			print = false;
			return x;
		}
	}

	return 0;
}

int32_t AoC2024_day14::get_aoc_day() {
	return 14;
}

int32_t AoC2024_day14::get_aoc_year() {
	return 2024;
}

void AoC2024_day14::tests() {
	uint64_t result;

	if (init({"p=0,4 v=3,-3", "p=6,3 v=-1,-3", "p=10,3 v=-1,2", "p=2,0 v=2,-1", "p=0,0 v=1,3", "p=3,0 v=-2,-2", "p=7,6 v=-1,-3", "p=3,0 v=-1,-2", "p=9,3 v=2,3",
			  "p=7,3 v=-1,2", "p=2,4 v=2,-3", "p=9,5 v=-3,-3"})) {
		result = get_safety_factor(100, C_WIDTH_TEST, C_HEIGHT_TEST); // 12
	}
}

bool AoC2024_day14::part1() {
	int64_t result = 0;

	result = get_safety_factor(100, C_WIDTH_PART, C_HEIGHT_PART);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2024_day14::part2() {
	int64_t result = 0;

	result = get_xmass_tree(100, C_WIDTH_PART, C_HEIGHT_PART);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2024_day14 day14;

	return day14.main_execution();
}
