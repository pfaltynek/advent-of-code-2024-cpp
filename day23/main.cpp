#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
#include <map>
#include <set>

class AoC2024_day23 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::map<std::string, std::set<std::string>> connections_;
	int64_t get_groups_count();
	std::string get_lan_party_password();
};

bool AoC2024_day23::init(const std::vector<std::string> lines) {
	connections_.clear();

	std::string left, right;

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (lines[i].find_first_not_of("-abcdefghijklmnopqrstuvwxyz") != std::string::npos) {
			std::cout << "Invalid connection content at line " << i + 1 << std::endl;
			return false;
		}

		if (lines[i].size() != 5) {
			std::cout << "Invalid connection length at line " << i + 1 << std::endl;
			return false;
		}

		if (lines[i][2] != '-') {
			std::cout << "Invalid connection format at line " << i + 1 << std::endl;
			return false;
		}

		left = lines[i].substr(0, 2);
		right = lines[i].substr(3, 2);

		connections_[left].emplace(right);
		connections_[right].emplace(left);
	}

	return true;
}

int64_t AoC2024_day23::get_groups_count() {
	int64_t result = 0;
	std::set<std::set<std::string>> triangles;
	std::set<std::string> triangle;

	triangles.clear();

	for (const auto& [key, value] : connections_) {
		for (const auto& item1 : value) {
			for (const auto& item2 : value) {
				if (item1 == item2) {
					continue;
				}

				if (connections_[item1].count(item2) != 0) {
					triangle.clear();
					triangle.emplace(key);
					triangle.emplace(item1);
					triangle.emplace(item2);

					if (triangles.count(triangle) == 0) {
						triangles.emplace(triangle);
					}
				}
			}
		}
	}

	for (const auto& triangle : triangles) {
		for (const auto& edge : triangle) {
			if (edge[0] == 't') {
				result++;
				break;
			}
		}
	}

	return result;
}

std::string AoC2024_day23::get_lan_party_password() {
	std::string password = "";
	std::set<std::string> party;
	uint8_t count = 0;

	for (const auto& [key, values] : connections_) {
		party.clear();
		party.emplace(key);

		for (const auto& value : values) {
			bool all = true;

			for(const auto& item : party) {
				if (connections_[item].count(value) == 0) {
					all = false;
					break;
				}
			}

			if (all) {
				party.emplace(value);
			}
		}

		if (party.size() > count) {
			count = party.size();
			password = join(party, ",");
		}
	}

	return password;
}
int32_t AoC2024_day23::get_aoc_day() {
	return 23;
}

int32_t AoC2024_day23::get_aoc_year() {
	return 2024;
}

void AoC2024_day23::tests() {
	int64_t result;

	if (init({"kh-tc", "qp-kh", "de-cg", "ka-co", "yn-aq", "qp-ub", "cg-tb", "vc-aq", "tb-ka", "wh-tc", "yn-cg",
			  "kh-ub", "ta-co", "de-co", "tc-td", "tb-wq", "wh-td", "ta-ka", "td-qp", "aq-cg", "wq-ub", "ub-vc",
			  "de-ta", "wq-aq", "wq-vc", "wh-yn", "ka-de", "kh-ta", "co-tc", "wh-qp", "tb-vc", "td-yn"})) {
		result = get_groups_count(); // 7
		std::string password = get_lan_party_password(); // "co,de,ka,ta"
	}
}

bool AoC2024_day23::part1() {
	int64_t result = 0;

	result = get_groups_count();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2024_day23::part2() {
	result2_ = get_lan_party_password();

	return true;
}

int main(void) {
	AoC2024_day23 day23;

	return day23.main_execution();
}
