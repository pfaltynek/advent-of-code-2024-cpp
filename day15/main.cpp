#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <set>

class AoC2024_day15 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::set<coord_str> walls_, boxes_;
	coord_str robot_, map_size_;
	std::string commands_;
	uint64_t get_gps_sum();
	uint64_t get_gps_sum2();
	coord_str get_direction_from_command(const char command);
};

bool AoC2024_day15::init(const std::vector<std::string> lines) {
	bool robot_found = false, map_complete = false;

	walls_.clear();
	boxes_.clear();
	commands_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (!map_complete) {
			if (lines[i].empty()) {
				map_complete = true;
				map_size_.y = i;

				continue;
			}

			map_size_.x = lines[i].size();

			if (lines[i].find_first_not_of("#.O@") != std::string::npos) {
				std::cout << "Unknown map symbol at line " << i + 1 << std::endl;
				return false;
			}

			for (uint32_t j = 0; j < lines[i].size(); j++) {
				switch (lines[i][j]) {
					case '#':
						walls_.emplace(coord_str(j, i));
						break;
					case '.':
						break;
					case 'O':
						boxes_.emplace(coord_str(j, i));
						break;
					case '@':
						if (robot_found) {
							std::cout << "Too much robot symbols (at line " << i + 1 << ")" << std::endl;
							return false;
						} else {
							robot_ = {j, i};
						}
						break;
				}
			}

		} else {
			if (lines[i].find_first_not_of("v<>^") != std::string::npos) {
				std::cout << "Unknown command symbol at line " << i + 1 << std::endl;
				return false;
			} else {
				commands_.append(lines[i]);
			}
		}
	}

	return true;
}

coord_str AoC2024_day15::get_direction_from_command(const char command) {
	switch (command) {
		case 'v':
			return coord_step_south;
		case '<':
			return coord_step_west;
		case '>':
			return coord_step_east;
		case '^':
			return coord_step_north;
		default:
			assert(false);
			return coord_step_north;
	}
}

uint64_t AoC2024_day15::get_gps_sum() {
	uint64_t result = 0;
	coord_str robot, direction, step, tmp;
	std::set<coord_str> boxes(boxes_);

	robot = robot_;

	for (size_t i = 0; i < commands_.size(); i++) {

		direction = get_direction_from_command(commands_[i]);
		step = robot + direction;

		if (walls_.count(step)) {
			continue;
		} else if (boxes.count(step)) {
			tmp = step;
			do {
				tmp = tmp + direction;
				if (walls_.count(tmp)) {
					break;
				} else if (boxes.count(tmp)) {
					continue;
				} else {
					boxes.emplace(tmp);
					boxes.erase(step);
					robot = step;
					break;
				}
			} while (true);
		} else {
			robot = step;
		}
	}

	for (auto& coord : boxes) {
		result += 100 * coord.y + coord.x;
	}

	return result;
}

uint64_t AoC2024_day15::get_gps_sum2() {
	uint64_t result = 0;
	coord_str robot, direction, step, tmp, tmp2;
	std::set<coord_str> walls;
	std::map<coord_str, bool> boxes;
	std::set<coord_str> line, next, wave;

	walls.clear();
	boxes.clear();

	robot = robot_;
	robot.x *= 2;

	for (auto& pos : walls_) {
		tmp = pos;
		tmp.x = pos.x * 2;
		walls.emplace(tmp);
		tmp.x++;
		walls.emplace(tmp);
	}

	for (auto& pos : boxes_) {
		tmp = pos;
		tmp.x = pos.x * 2;
		boxes[tmp] = false;
		tmp.x++;
		boxes[tmp] = true;
	}

	for (size_t i = 0; i < commands_.size(); i++) {
		direction = get_direction_from_command(commands_[i]);
		step = robot + direction;

		if (walls.count(step)) {
			continue;
		} else if (boxes.count(step)) {
			wave.clear();
			line.clear();
			next.clear();

			tmp = step;
			bool wall = false;

			if ((direction == coord_step_south) || (direction == coord_step_north)) {
				line.emplace(step);

				if (boxes[step]) {
					tmp.x--;
				} else {
					tmp.x++;
				}

				if (!boxes.count(tmp)) {
					assert(false);
				} else if (boxes[step] == boxes[tmp]) {
					assert(false);
				} else {
					line.emplace(tmp);
					wave.insert(line.begin(), line.end());

					while (true) {
						for (auto& pos : line) {
							tmp = direction + pos;

							if (walls.count(tmp)) {
								wall = true;
								break;
							} else if (boxes.count(tmp)) {
								next.emplace(tmp);

								if (boxes[tmp] != boxes[pos]) {
									tmp2 = tmp;

									if (boxes[tmp]) {
										tmp2.x--;
									} else {
										tmp2.x++;
									}

									if (!boxes.count(tmp2)) {
										assert(false);
									} else if (boxes[tmp] == boxes[tmp2]) {
										assert(false);
									} else {
										next.emplace(tmp2);
									}
								}
							}
						}

						if (wall) {
							break;
						} else if (next.empty()) {
							while (!wave.empty()) {
								for (auto& coord : wave) {
									tmp = direction + coord;
									if (!boxes.count(tmp)) {
										bool box_type = boxes[coord];
										boxes.erase(coord);
										boxes[tmp] = box_type;
										wave.erase(coord);
										break;
									}
								}
							}
							robot = step;
							break;
						} else {
							line = next;
							next.clear();
							wave.insert(line.begin(), line.end());
						}
					}
				}
			} else if ((direction == coord_step_east) || (direction == coord_step_west)) {
				bool box_type = (direction == coord_step_west);
				uint32_t box_count = 0;
				tmp = step;

				while (true) {
					if (walls.count(tmp)) {
						wall = true;
						break;
					} else if (boxes.count(tmp)) {
						if (boxes[tmp] != box_type) {
							assert(false);
						}
						box_count++;
						box_type = !box_type;
						line.emplace(tmp);
						tmp = tmp + direction;
					} else {
						if (box_count % 2 == 0) {

							while (!line.empty()) {
								for (auto& coord : line) {
									tmp = direction + coord;
									if (!boxes.count(tmp)) {
										bool box_type = boxes[coord];
										boxes.erase(coord);
										boxes[tmp] = box_type;
										line.erase(coord);
										break;
									}
								}
							}
							robot = step;
							break;
						}
					}
				}

			} else {
				assert(false);
			}
		} else {
			robot = step;
		}
	}

	for (auto& pair : boxes) {
		if (!pair.second) {
			result += 100 * pair.first.y + pair.first.x;
		}
	}

	return result;
}

int32_t AoC2024_day15::get_aoc_day() {
	return 15;
}

int32_t AoC2024_day15::get_aoc_year() {
	return 2024;
}

void AoC2024_day15::tests() {
	uint64_t result;

	if (init({"########", "#..O.O.#", "##@.O..#", "#...O..#", "#.#.O..#", "#...O..#", "#......#", "########", "", "<^^>>>vv<v>>v<<"})) {
		result = get_gps_sum(); // 2028
	}

	if (init({"#######", "#...#.#", "#.....#", "#..OO@#", "#..O..#", "#.....#", "#######", "", "<vv<<^^<<^^"})) {
		result = get_gps_sum2(); // 618
	}

	if (init({"##########",
			  "#..O..O.O#",
			  "#......O.#",
			  "#.OO..O.O#",
			  "#..O@..O.#",
			  "#O#..O...#",
			  "#O..O..O.#",
			  "#.OO.O.OO#",
			  "#....O...#",
			  "##########",
			  "",
			  "<vv>^<v^>v>^vv^v>v<>v^v<v<^vv<<<^><<><>>v<vvv<>^v^>^<<<><<v<<<v^vv^v>^",
			  "vvv<<^>^v^^><<>>><>^<<><^vv^^<>vvv<>><^^v>^>vv<>v<<<<v<^v>^<^^>>>^<v<v",
			  "><>vv>v^v^<>><>>>><^^>vv>v<^^^>>v^v^<^^>v^^>v^<^v>v<>>v^v^<v>v^^<^^vv<",
			  "<<v<^>>^^^^>>>v^<>vvv^><v<<<>^^^vv^<vvv>^>v<^^^^v<>^>vvvv><>>v^<<^^^^^",
			  "^><^><>>><>^^<<^^v>>><^<v>^<vv>>v>>>^v><>^v><<<<v>>v<v<v>vvv>^<><<>^><",
			  "^>><>^v<><^vvv<^^<><v<<<<<><^v<<<><<<^^<v<^^^><^>>^<v^><<<^>>^v<v^v<v^",
			  ">^>>^v>vv>^<<^v<>><<><<v<<v><>v<^vv<<<>^^v^>^^>>><<^v>>v^v><^^>>^<>vv^",
			  "<><^^>^^^<><vvvvv^v<v<<>^v<v>v<<^><<><<><<<^^<<<^<<>><<><^^^>^^<>^>v<>",
			  "^^>vv<^v^v<vv>^<><v<^v>^^^>>>^^vvv^>vvv<>>>^<^>>>>>^<<^v>^vvv<>^<><<v>",
			  "v^^>>><<^^<>>^v^<v^vv<>v^<<>^<^v^v><^<<<><<^<v><v<>vv>>v><v^<vv<>v^<<^"})) {
		result = get_gps_sum();	 // 10092
		result = get_gps_sum2(); // 9021
	}
}

bool AoC2024_day15::part1() {
	int64_t result = 0;

	result = get_gps_sum();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2024_day15::part2() {
	int64_t result = 0;

	result = get_gps_sum2();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2024_day15 day15;

	return day15.main_execution();
}
