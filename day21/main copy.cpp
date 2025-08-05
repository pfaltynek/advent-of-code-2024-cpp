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
	std::map<char, std::map<char, std::set<std::string>>> steps_numeric_, steps_arrow_;
	std::map<char, std::map<char, size_t>> arrow_sizes_;
	std::map<std::string, std::set<std::string>> cache_;
	void prepare_steps_maps();
	int64_t get_numeric_part(const std::string code);
	int64_t get_code_complexities_sum(const bool part2);
	std::map<char, std::map<char, std::set<std::string>>> get_steps_map(std::vector<std::string> keypad);
	std::map<char, std::map<char, size_t>> get_steps_sizes_map(const size_t repeats, std::map<char, std::map<char, std::set<std::string>>>& steps);
	std::set<std::string> translate(std::map<char, std::map<char, std::set<std::string>>>& steps, const std::set<std::string> codes);
	size_t get_shortest_code_length(std::set<std::string>& codes);
	size_t get_shortest_codes(std::set<std::string>& codes);
	bool is_optimized(const std::string path);
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

int64_t AoC2024_day21::get_numeric_part(const std::string code) {
	int64_t result = 0;

	for (uint32_t i = 0; i < code.size(); i++) {
		if (code[i] >= '0' && code[i] <= '9') {
			result = result * 10 + code[i] - '0';
		}
	}

	return result;
}

size_t AoC2024_day21::get_shortest_code_length(std::set<std::string>& codes) {
	size_t size = SIZE_MAX;

	for (auto& code : codes) {
		if (code.size() < size) {
			size = code.size();
		}
	}

	return size;
}

size_t AoC2024_day21::get_shortest_codes(std::set<std::string>& codes) {
	std::string code;
	
	size_t size = get_shortest_code_length(codes);

	for (auto it = codes.begin(); it != codes.end();) {
		if (it->size() > size) {
			it = codes.erase(it);
		} else {
			++it;
		}
	}

	return size;
}

bool AoC2024_day21::is_optimized(const std::string path) {

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

std::set<std::string> AoC2024_day21::translate(std::map<char, std::map<char, std::set<std::string>>>& steps, const std::set<std::string> codes) {
	std::set<std::string> subresult, result, tmp;
	char start = 'A';

	result.clear();

	for (auto& code : codes) {
		subresult.clear();
		for (size_t i = 0; i < code.size(); i++) {

			if (steps.count(start)) {
				if (steps[start].count(code[i])) {
					tmp.clear();

					if (subresult.empty()) {
						subresult = steps[start][code[i]];
					} else {
						for (auto& step : steps[start][code[i]]) {
							for (auto& subres : subresult) {
								tmp.emplace(subres + step);
							}
						}
						subresult.swap(tmp);
					}

					start = code[i];
				} else {
					assert(false);
				}
			} else {
				assert(false);
			}
		}
		result.insert(subresult.begin(), subresult.end());
	}

	return result;
}

void AoC2024_day21::prepare_steps_maps() {
	steps_numeric_ = get_steps_map(C_NUMERIC_KEYPAD);
	steps_arrow_ = get_steps_map(C_ARROW_KEYPAD);
	// arrow_sizes_ = get_steps_sizes_map(1, steps_arrow_);
}

std::map<char, std::map<char, std::set<std::string>>> AoC2024_day21::get_steps_map(std::vector<std::string> keypad) {
	std::map<char, std::map<char, std::set<std::string>>> result;
	std::map<char, coord_str> key_coords;
	std::queue<state_str> q;
	std::set<coord_str> legal;
	coord_str from, to;
	state_str curr, next;
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

			while (!q.empty()) {
				curr = q.front();
				q.pop();

				if (!legal.count(curr.pos)) {
					continue;
				}
				if (curr.pos == to) {
					if (!is_optimized(curr.path)) {
						continue;
					}

					curr.path += 'A';
					if (result.count(all_keys[i])) {
						if (result[all_keys[i]].count(all_keys[j])) {
							first = *(result[all_keys[i]][all_keys[j]].begin());
							if (first.size() > curr.path.size()) {
								result[all_keys[i]][all_keys[j]].clear();
								result[all_keys[i]][all_keys[j]].emplace(curr.path);
							} else if (first.size() == curr.path.size()) {
								result[all_keys[i]][all_keys[j]].emplace(curr.path);
							}
						} else {
							result[all_keys[i]][all_keys[j]].emplace(curr.path);
						}
					} else {
						result[all_keys[i]][all_keys[j]].emplace(curr.path);
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
		}
	}

	return result;
}

std::map<char, std::map<char, size_t>> AoC2024_day21::get_steps_sizes_map(const size_t repeats, std::map<char, std::map<char, std::set<std::string>>>& steps) {
	std::map<char, std::map<char, size_t>> result;
	size_t size, tmp;

	result.clear();

	for (auto& start : steps) {
		for (auto& end : start.second) {
			size = SIZE_MAX;

			for (auto& str : end.second) {
				tmp = str.size();
				if (tmp < size) {
					size = tmp;
				}
			}

			result[start.first][end.first] = size;
		}
	}

	return result;
}

int64_t AoC2024_day21::get_code_complexities_sum(const bool part2) {
	int64_t result = 0, size, x;
	std::set<std::string> subresult, tmp, tmp2;
	char start;
	std::set<std::map<std::string, int32_t>> packed;

	if (!part2) {
		prepare_steps_maps();
	}


	for (uint32_t i = 0; i < codes_.size(); i++) {
		packed.clear();

		
	}



	for (uint32_t i = 0; i < codes_.size(); i++) {
		tmp = {codes_[i]};
		subresult = translate(steps_numeric_, tmp);
		tmp.clear();
		tmp.swap(subresult);

		subresult = translate(steps_arrow_, tmp);

		tmp.clear();
		tmp.swap(subresult);

		size = INT64_MAX;

		for (auto& code : tmp) {
			start = 'A';
			x = 0;

			for (size_t i = 0; i < code.size(); i++) {
				if (arrow_sizes_.count(start)) {
					if (arrow_sizes_[start].count(code[i])) {
						x += arrow_sizes_[start][code[i]];
						start = code[i];
					} else {
						assert(false);
					}
				} else {
					assert(false);
				}
			}

			if (x < size) {
				size = x;
			}
		}

		size *= get_numeric_part(codes_[i]);
		result += size;
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
