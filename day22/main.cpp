#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <array>
#include <map>
#include <set>

const int64_t C_MULTIPLY1 = 64;
const int64_t C_DIVIDE1 = 32;
const int64_t C_MULTIPLY2 = 2048;
const int64_t C_MODULO = 16777216;
const int64_t C_GENERATION_COUNT = 2000;
const char C_ZERO_DIFF_CHAR = 'L';

class AoC2024_day22 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<int64_t> initial_secret_numbers_, secret_numbers_;
	std::set<std::string> sequences_;
	std::vector<std::string> prices_, changes_;
	int64_t get_next_secret_number(const int64_t secret);
	int64_t get_sum_of_secret_numbers(const int64_t generation_count);
	int64_t get_max_bananas_count();
};

bool AoC2024_day22::init(const std::vector<std::string> lines) {
	initial_secret_numbers_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (lines[i].find_first_not_of("0123456789") != std::string::npos) {
			std::cout << "Invalid code content at line " << i + 1 << std::endl;
			return false;
		}

		initial_secret_numbers_.push_back(std::stoi(lines[i]));
	}

	return true;
}

int64_t AoC2024_day22::get_next_secret_number(const int64_t secret) {
	int64_t result;

	result = ((secret * C_MULTIPLY1) ^ secret) % C_MODULO;
	result = ((result / C_DIVIDE1) ^ result) % C_MODULO;
	result = ((result * C_MULTIPLY2) ^ result) % C_MODULO;

	return result;
}

int64_t AoC2024_day22::get_sum_of_secret_numbers(const int64_t generation_count) {
	secret_numbers_ = initial_secret_numbers_;
	int64_t result = 0;
	int8_t prev, price, diff;
	std::string diffs, prices;

	prices_.clear();
	changes_.clear();
	sequences_.clear();

	for (size_t j = 0; j < secret_numbers_.size(); j++) {
		diffs.clear();
		prices.clear();

		prev = secret_numbers_[j] % 10;

		for (int64_t i = 0; i < generation_count; i++) {
			secret_numbers_[j] = get_next_secret_number(secret_numbers_[j]);
			price = secret_numbers_[j] % 10;

			diff = price - prev;
			diffs += diff + C_ZERO_DIFF_CHAR;
			prices += price + C_ZERO_DIFF_CHAR;

			if ((i >= 4) && (i < generation_count - 4)) {
				std::string seq = diffs.substr(i - 4, 4);

				sequences_.insert(seq);
			}

			prev = price;
		}
		result += secret_numbers_[j];

		changes_.push_back(diffs);
		prices_.push_back(prices);
	}

	return result;
}

int64_t AoC2024_day22::get_max_bananas_count() {
	int64_t max_bananas = 0;

	for (const auto& seq : sequences_) {
		int64_t bananas = 0;

		for (size_t i = 0; i < changes_.size(); i++) {
			size_t pos;

			pos = changes_[i].find(seq);

			if (pos != std::string::npos) {
				int8_t price = prices_[i][pos + seq.size() - 1] - C_ZERO_DIFF_CHAR;
				bananas += price;
			}
		}

		if (bananas > max_bananas) {
			max_bananas = bananas;
		}
	}

	return max_bananas;
}

int32_t AoC2024_day22::get_aoc_day() {
	return 22;
}

int32_t AoC2024_day22::get_aoc_year() {
	return 2024;
}

void AoC2024_day22::tests() {
	int64_t result;

	result = 123;
	result = get_next_secret_number(result); // 15887950
	result = get_next_secret_number(result); // 16495136
	result = get_next_secret_number(result); // 527345
	result = get_next_secret_number(result); // 704524
	result = get_next_secret_number(result); // 1553684
	result = get_next_secret_number(result); // 12683156
	result = get_next_secret_number(result); // 11100544
	result = get_next_secret_number(result); // 12249484
	result = get_next_secret_number(result); // 7753432
	result = get_next_secret_number(result); // 5908254

	if (init({"123"})) {
		result = get_sum_of_secret_numbers(10); // 5908254
	}

	if (init({"1", "10", "100", "2024"})) {
		result = get_sum_of_secret_numbers(C_GENERATION_COUNT); // 37327623
		// 1: 8685429, 10: 4700978, 100: 15273692, 2024: 8667524
	}

	if (init({"1", "2", "3", "2024"})) {
		result = get_sum_of_secret_numbers(C_GENERATION_COUNT); // 37990510
		result = get_max_bananas_count(); // 23
	}
}

bool AoC2024_day22::part1() {
	int64_t result = 0;

	result = get_sum_of_secret_numbers(C_GENERATION_COUNT);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2024_day22::part2() {
	int64_t result;

	result = get_max_bananas_count();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2024_day22 day22;

	return day22.main_execution();
}
