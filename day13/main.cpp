#include "./../common/aoc.hpp"
#include <sstream>

struct machine_str {
	int64_t ax, ay, bx, by, px, py;
};

class AoC2024_day13 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<machine_str> machines_;
	uint64_t get_minimal_tokens_to_win_all_possible_prizes(const bool part2);
	uint64_t solve(const machine_str machine, const bool part2);
};

bool AoC2024_day13::init(const std::vector<std::string> lines) {
	std::stringstream ss;
	machine_str mach;
	bool complete;

	machines_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		ss.clear();
		ss.str(lines[i]);

		switch (i % 4) {
			case 0:
				complete = false;
				ss.ignore(12); // "Button A: X+"
				ss >> mach.ax;
				ss.ignore(4); // ", Y+"
				ss >> mach.ay;
				break;
			case 1:
				ss.ignore(12); // "Button A: X+"
				ss >> mach.bx;
				ss.ignore(4); // ", Y+"
				ss >> mach.by;
				break;
			case 2:
				ss.ignore(9); // "Prize: X="
				ss >> mach.px;
				ss.ignore(4); // ", Y="
				ss >> mach.py;
				complete = true;
				break;
			case 3:
				complete = false;
				break;
		}

		if (complete) {
			machines_.push_back(mach);
		}
	}

	return true;
}

uint64_t AoC2024_day13::solve(const machine_str machine, const bool part2) {
	int64_t x1, x2, x3, y1, y2, y3, z1, z2, z3, a, b, prizex, prizey;

	// ax*a + bx*b = px
	// ay*a + by*b = py
	if (part2) {
		prizex = machine.px + 10000000000000;
		prizey = machine.py + 10000000000000;
	} else {
		prizex = machine.px;
		prizey = machine.py;
	}

	x1 = machine.ax * (-1 * machine.by);
	x2 = machine.bx * (-1 * machine.by);
	y1 = machine.ay * (machine.bx);
	y2 = machine.by * (machine.bx);
	x3 = prizex * (-1 * machine.by);
	y3 = prizey * (machine.bx);

	z1 = x1 + y1;
	z2 = x2 + y2;
	z3 = x3 + y3;

	if (z2 != 0) {
		assert(false);
	}

	a = z3 / z1;
	b = (prizey - (a * machine.ay)) / machine.by;

	if (((a * machine.ax) + (b * machine.bx) == prizex) && ((a * machine.ay) + (b * machine.by) == prizey)) {
		return a * 3 + b;
	}

	return 0;
}

uint64_t AoC2024_day13::get_minimal_tokens_to_win_all_possible_prizes(const bool part2) {
	uint64_t result = 0;

	for (size_t i = 0; i < machines_.size(); i++) {
		result += solve(machines_[i], part2);
	}

	return result;
}

int32_t AoC2024_day13::get_aoc_day() {
	return 13;
}

int32_t AoC2024_day13::get_aoc_year() {
	return 2024;
}

void AoC2024_day13::tests() {
	uint64_t result;

	if (init({"Button A: X+94, Y+34", "Button B: X+22, Y+67", "Prize: X=8400, Y=5400", "", "Button A: X+26, Y+66", "Button B: X+67, Y+21",
			  "Prize: X=12748, Y=12176", "", "Button A: X+17, Y+86", "Button B: X+84, Y+37", "Prize: X=7870, Y=6450", "", "Button A: X+69, Y+23",
			  "Button B: X+27, Y+71", "Prize: X=18641, Y=10279"})) {
		result = get_minimal_tokens_to_win_all_possible_prizes(false); // 480
		result = get_minimal_tokens_to_win_all_possible_prizes(true);  // ???
	}
}

bool AoC2024_day13::part1() {
	int64_t result = 0;

	result = get_minimal_tokens_to_win_all_possible_prizes(false);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2024_day13::part2() {
	int64_t result = 0;

	result = get_minimal_tokens_to_win_all_possible_prizes(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2024_day13 day13;

	return day13.main_execution();
}
