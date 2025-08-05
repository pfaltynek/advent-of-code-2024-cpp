#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <queue>
#include <set>

std::vector<std::string> C_NUMERIC_KEYPAD = {"789", "456", "123", " 0A"};
std::vector<std::string> C_ARROW_KEYPAD = {" ^A", "<v>"};

struct state_str {
	coord_str pos;
	std::string path;
	std::set<coord_str> visited;
};

// To get part2 result the app optimalization is needed. This is done by
// removing all paths that are not optimal. Even if the found paths have same length
// their real length is different during the 3rd or 4th iteration level.
// Thats why following table is used (Reddit user solution,
// see https://www.reddit.com/r/adventofcode/comments/1hjgyps/2024_day_21_part_2_i_got_greedyish/).
// It is applied only for diagonal transitions due the importance of order
// horizontal (U/D) and vertical (L/R) transitions.
//
//	Path		U/D	L/R	Best Order
//	UP-LEFT		U	L	L + U
//	DOWN-LEFT	D	L	L + D
//	DOWN-RIGHT	D	R	D + R
//	UP-RIGHT	U	R	U + R
//
// Here is sample of hardcoded optimal paths (another Reddit user veydar_ has provided in
// https://www.reddit.com/r/adventofcode/comments/1hj2odw/2024_day_21_solutions/):
//	"A" {"A" [] "^" ["<"] ">" ["v"] "v" ["<" "v"] "<" ["v" "<" "<"]}
//	"^" {"^" [] "A" [">"] ">" ["v" ">"] "v" ["v"] "<" ["v" "<"]}
//	"<" {"<" [] "v" [">"] "^" [">" "^"] ">" [">" ">"] "A" [">" ">" "^"]}
//	"v" {"v" [] "<" ["<"] ">" [">"] "^" ["^"] "A" ["^" ">"]}
//	">" {">" [] "A" ["^"] "^" ["<" "^"] "v" ["<"] "<" ["<" "<"]}

//	"7" {"7" [] "8" [">"] "9" [">" ">"]
//		"4" ["v"] "5" ["v" ">"] "6" ["v" ">" ">"]
//		"1" ["v" "v"] "2" ["v" "v" ">"] "3" ["v" "v" ">" ">"]
//		"0" [">" "v" "v" "v"] "A" [">" ">" "v" "v" "v"]}
//
//	"8" {"7" ["<"] "8" [] "9" [">"]
//		"4" ["<" "v"] "5" ["v"] "6" ["v" ">"]
//		"1" ["<" "v" "v"] "2" ["v" "v"] "3" ["v" "v" ">"]
//		"0" ["v" "v" "v"] "A" ["v" "v" "v" ">"]}
//
//	"9" {"7" ["<" "<"] "8" ["<"] "9" []
//		"4" ["<" "<" "v"] "5" ["<" "v"] "6" ["v"]
//		"1" ["<" "<" "v" "v"] "2" ["<" "v" "v"] "3" ["v" "v"]
//		"0" ["<" "v" "v" "v"] "A" ["v" "v" "v"]}
//
//	"4" {"7" ["^"] "8" ["^" ">"] "9" ["^" ">" ">"]
//		"4" [] "5" [">"] "6" [">" ">"]
//		"1" ["v"] "2" ["v" ">"] "3" ["v" ">" ">"]
//		"0" [">" "v" "v"] "A" [">" ">" "v" "v"]}
//
//	"5" {"7" ["<" "^"] "8" ["^"] "9" ["^" ">"]
//		"4" ["<"] "5" [] "6" [">"]
//		"1" ["<" "v"] "2" ["v"] "3" ["v" ">"]
//		"0" ["v" "v"] "A" ["v" "v" ">"]}
//
//	"6" {"7" ["<" "<" "^"] "8" ["<" "^"] "9" ["^"]
//		"4" ["<" "<"] "5" ["<"] "6" []
//		"1" ["<" "<" "v"] "2" ["<" "v"] "3" ["v"]
//		"0" ["<" "v" "v"] "A" ["v" "v"]}
//
//	"1" {"7" ["^" "^"] "8" ["^" "^" ">"] "9" ["^" "^" ">" ">"]
//		"4" ["^"] "5" ["^" ">"] "6" ["^" ">" ">"]
//		"1" [] "2" [">"] "3" [">" ">"]
//		"0" [">" "v"] "A" [">" ">" "v"]}
//
//	"2" {"7" ["<" "^" "^"] "8" ["^" "^"] "9" ["^" "^" ">"]
//		"4" ["<" "^"] "5" ["^"] "6" ["^" ">"]
//		"1" ["<"] "2" [] "3" [">"]
//		"0" ["v"] "A" ["v" ">"]}
//
//	"3" {"7" ["<" "<" "^" "^"] "8" ["<" "^" "^"] "9" ["^" "^"]
//		"4" ["<" "<" "^"] "5" ["<" "^"] "6" ["^"]
//		"1" ["<" "<"] "2" ["<"] "3" []
//		"0" ["<" "v"] "A" ["v"]}
//
//	"0" {"7" ["^" "^" "^" "<"] "8" ["^" "^" "^"] "9" ["^" "^" "^" ">"]
//		"4" ["^" "^" "<"] "5" ["^" "^"] "6" ["^" "^" ">"]
//		"1" ["^" "<"] "2" ["^"] "3" ["^" ">"]
//		"0" [] "A" [">"]}
//
//	"A" {"7" ["^" "^" "^" "<" "<"] "8" ["<" "^" "^" "^"] "9" ["^" "^" "^"]
//		"4" ["^" "^" "<" "<"] "5" ["^" "^" "<"] "6" ["^" "^"]
//		"1" ["^" "<" "<"] "2" ["^" "<"] "3" ["^"]
//		"0" ["<"] "A" []}

class AoC2024_day21 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<std::string> codes_;
	std::map<char, std::map<char, std::string>> steps_numeric_, steps_arrow_;
	std::map<std::string, std::map<std::string, int64_t>> cache_;
	int64_t get_code_complexities_sum(const bool part2);
	int64_t get_numeric_part(const std::string code);
	std::map<char, std::map<char, std::string>> get_steps_map(std::vector<std::string> keypad);
	std::map<std::string, int64_t> numeric2arrows(const std::string code);
	std::map<std::string, int64_t> arrows2arrows(const std::string code);
	int64_t get_packed_size(const std::map<std::string, int64_t>& packed);
	void prepare_steps_maps();
	bool is_path_optimized(const std::string path);
};

bool AoC2024_day21::init(const std::vector<std::string> lines) {
	codes_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (lines[i].find_first_not_of("0123456789A") != std::string::npos) {
			std::cout << "Invalid code content at line " << i + 1 << std::endl;
			return false;
		}

		codes_.push_back(lines[i]);
	}

	return true;
}

bool AoC2024_day21::is_path_optimized(const std::string path) {

	if (path.size() > 2) {
		for (size_t i = 0; i < path.size() - 2; i++) {
			if (path[i] != path[i + 1]) {
				for (size_t j = i + 2; j < path.size(); j++) {
					if (path[i] == path[j]) {
						return false;
					}
				}
			}
		}
	}

	return true;
}

std::map<char, std::map<char, std::string>> AoC2024_day21::get_steps_map(std::vector<std::string> keypad) {
	std::map<char, std::map<char, std::string>> result;
	std::map<char, coord_str> key_coords;
	std::queue<state_str> q;
	std::set<coord_str> legal;
	coord_str from, to;
	state_str curr, next;
	std::set<std::string> paths;
	std::map<char, coord_str> directions = {{'v', coord_step_south}, {'^', coord_step_north}, {'<', coord_step_west}, {'>', coord_step_east}};

	std::string all_keys = "", first;
	result.clear();
	legal.clear();

	for (uint32_t i = 0; i < keypad.size(); i++) {
		for (uint32_t j = 0; j < keypad[i].size(); j++) {
			if (keypad[i][j] == ' ') {
				continue;
			}

			all_keys += keypad[i][j];

			key_coords[keypad[i][j]] = {j, i};
			legal.emplace(coord_str({j, i}));
		}
	}

	for (uint32_t i = 0; i < all_keys.size(); i++) {
		from = key_coords[all_keys[i]];

		for (uint32_t j = 0; j < all_keys.size(); j++) {
			to = key_coords[all_keys[j]];

			q = {};
			curr.path = "";
			curr.pos = from;
			curr.visited = {};

			q.push(curr);

			paths.clear();

			while (!q.empty()) {
				curr = q.front();
				q.pop();

				if (!legal.count(curr.pos)) {
					continue;
				}
				if (curr.pos == to) {
					if (!is_path_optimized(curr.path)) {
						continue;
					}

					curr.path += 'A';

					if (paths.empty()) {
						paths.emplace(curr.path);
					} else {
						first = *(paths.begin());

						if (first.size() > curr.path.size()) {
							paths.clear();
							paths.emplace(curr.path);
						} else if (first.size() == curr.path.size()) {
							paths.emplace(curr.path);
						}
					}
					continue;
				}

				if (curr.visited.count(curr.pos)) {
					continue;
				}

				curr.visited.emplace(curr.pos);

				for (auto& dir : directions) {
					next.pos = curr.pos + dir.second;
					next.path = curr.path + dir.first;
					next.visited = curr.visited;

					if (legal.count(next.pos)) {
						q.push(next);
					}
				}
			}

			if (paths.size() == 1) {
				result[all_keys[i]][all_keys[j]] = *paths.begin();
			} else {
				bool found = false;

				for (auto& path : paths) {
					if (path.find("<^") != std::string::npos) {
						result[all_keys[i]][all_keys[j]] = path;
						found = true;
						break;
					} else if (path.find("<v") != std::string::npos) {
						result[all_keys[i]][all_keys[j]] = path;
						found = true;
						break;
					} else if (path.find("v>") != std::string::npos) {
						result[all_keys[i]][all_keys[j]] = path;
						found = true;
						break;
					} else if (path.find("^>") != std::string::npos) {
						result[all_keys[i]][all_keys[j]] = path;
						found = true;
						break;
					}
				}

				if (!found) {
					assert(false);
				}
			}
		}
	}

	return result;
}

void AoC2024_day21::prepare_steps_maps() {
	steps_numeric_ = get_steps_map(C_NUMERIC_KEYPAD);
	steps_arrow_ = get_steps_map(C_ARROW_KEYPAD);
}

int64_t AoC2024_day21::get_packed_size(const std::map<std::string, int64_t>& packed) {
	int64_t result = 0;

	for (auto& part : packed) {
		result += part.first.size() * part.second;
	}

	return result;
}

std::map<std::string, int64_t> AoC2024_day21::arrows2arrows(const std::string code) {
	std::map<std::string, int64_t> result, x1;

	if (cache_.count(code)) {
		result = cache_[code];
	} else {

		char start = 'A';

		result.clear();

		for (size_t i = 0; i < code.size(); i++) {

			if (steps_arrow_.count(start)) {
				if (steps_arrow_[start].count(code[i])) {

					result[steps_arrow_[start][code[i]]]++;

					start = code[i];
				} else {
					assert(false);
				}
			} else {
				assert(false);
			}
		}

		cache_[code] = result;
	}

	return result;
}

std::map<std::string, int64_t> AoC2024_day21::numeric2arrows(const std::string code) {
	std::map<std::string, int64_t> result;
	std::vector<std::map<std::string, int64_t>> subresult_, x1, x2;
	std::map<std::string, int64_t> tmp;
	char start = 'A';

	result.clear();

	for (size_t i = 0; i < code.size(); i++) {

		if (steps_numeric_.count(start)) {
			if (steps_numeric_[start].count(code[i])) {
				result[steps_numeric_[start][code[i]]]++;

				start = code[i];
			} else {
				assert(false);
			}
		} else {
			assert(false);
		}
	}

	return result;
}

int64_t AoC2024_day21::get_numeric_part(const std::string code) {
	int64_t result = 0;

	for (uint32_t i = 0; i < code.size(); i++) {
		if (code[i] >= '0' && code[i] <= '9') {
			result = result * 10 + code[i] - '0';
		}
	}

	return result;
}

int64_t AoC2024_day21::get_code_complexities_sum(const bool part2) {
	int64_t result = 0, size, numeric, levels = 25;
	std::map<std::string, int64_t> x1, x2, x3;

	if (!part2) {
		prepare_steps_maps();
		cache_.clear();
		levels = 2;
	}

	for (uint32_t i = 0; i < codes_.size(); i++) {

		x1 = numeric2arrows(codes_[i]);

		for (int64_t l = 0; l < levels; l++) {

			x3.clear();

			for (auto& pair : x1) {

				x2 = arrows2arrows(pair.first);

				for (auto& subpair : x2) {
					x3[subpair.first] += (subpair.second * pair.second);
				}
			}

			x1.swap(x3);
		}

		if (!x1.empty()) {
			size = get_packed_size(static_cast<const std::map<std::string, int64_t>>(x1));
		} else {
			size = 0;
		}

		numeric = get_numeric_part(codes_[i]);
		result += size * numeric;
	}

	return result;
}

int32_t AoC2024_day21::get_aoc_day() {
	return 21;
}

int32_t AoC2024_day21::get_aoc_year() {
	return 2024;
}

void AoC2024_day21::tests() {
	int64_t result;

	if (init({"029A", "980A", "179A", "456A", "379A"})) {
		result = get_code_complexities_sum(false); // 126384
	}
}

bool AoC2024_day21::part1() {
	int64_t result = 0;

	result = get_code_complexities_sum(false);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2024_day21::part2() {
	int64_t result;

	result = get_code_complexities_sum(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2024_day21 day21;

	return day21.main_execution();
}
