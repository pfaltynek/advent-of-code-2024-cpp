#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
#include <sstream>

class AoC2024_day17 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<uint8_t> program_;
	uint32_t ip_;
	std::vector<uint64_t> registers_, regs_backup_;
	std::string get_program_output();
	uint64_t get_a_register_init_value();
	bool get_program_step(uint32_t& ip, std::vector<uint64_t>& registers, bool& was_output, uint8_t& output);
	uint64_t get_combo_operand(uint8_t operand);
	std::vector<uint8_t> run_program();
};

bool AoC2024_day17::init(const std::vector<std::string> lines) {
	std::vector<std::string> tokens;
	bool ok;

	registers_ = {0, 0, 0};
	ip_ = 0;
	program_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		switch (i) {
			case 0:
				ok = false;
				tokens = split(lines[i], " ");
				if (tokens.size() == 3) {
					if ((tokens[0] == "Register") && (tokens[1] == "A:")) {
						registers_[0] = std::stoi(tokens[2]);
						ok = true;
					}
				}

				if (!ok) {
					std::cout << "Invalid Register A input format" << std::endl;
					return false;
				}
				break;
			case 1:
				ok = false;
				tokens = split(lines[i], " ");
				if (tokens.size() == 3) {
					if ((tokens[0] == "Register") && (tokens[1] == "B:")) {
						registers_[1] = std::stoi(tokens[2]);
						ok = true;
					}
				}

				if (!ok) {
					std::cout << "Invalid Register B input format" << std::endl;
					return false;
				}
				break;
			case 2:
				ok = false;
				tokens = split(lines[i], " ");
				if (tokens.size() == 3) {
					if ((tokens[0] == "Register") && (tokens[1] == "C:")) {
						registers_[2] = std::stoi(tokens[2]);
						ok = true;
					}
				}

				if (!ok) {
					std::cout << "Invalid Register C input format" << std::endl;
					return false;
				}
				break;
			case 3:
				// empty line - ignore it
				break;
			case 4:
				ok = false;
				tokens = split(lines[i], " ");
				if (tokens.size() == 2) {
					if (tokens[0] == "Program:") {
						tokens = split(tokens[1], ",");

						for (size_t i = 0; i < tokens.size(); i++) {
							program_.push_back(std::stoi(tokens[i]));
						}

						ok = true;
					}
				}

				if (!ok) {
					std::cout << "Invalid Program input format" << std::endl;
					return false;
				}
				break;
		}
	}

	regs_backup_ = registers_;

	return true;
}

uint64_t AoC2024_day17::get_combo_operand(uint8_t operand) {
	switch (operand) {
		case 0:
		case 1:
		case 2:
		case 3:
			return operand;

		case 4:
			return registers_[0];

		case 5:
			return registers_[1];

		case 6:
			return registers_[2];

		case 7:
		default:
			assert(false);
			return 0;
	}
}

bool AoC2024_day17::get_program_step(uint32_t& ip, std::vector<uint64_t>& registers, bool& was_output, uint8_t& output) {
	uint8_t opcode, operand;
	uint64_t combo_operand;

	was_output = false;

	if (ip < program_.size() - 1) {
		opcode = program_[ip++];
		operand = program_[ip++];

		switch (opcode) {
			case 0: // adv
				combo_operand = get_combo_operand(operand);

				registers[0] = registers[0] >> combo_operand;
				break;
			case 1: // bxl
				registers[1] ^= static_cast<uint64_t>(operand);
				break;
			case 2: // bst
				combo_operand = get_combo_operand(operand);
				registers[1] = combo_operand % 8;
				break;
			case 3: // jnz
				if (registers[0]) {
					ip = operand;
				}
				break;
			case 4: // bxc
				registers[1] ^= registers[2];
				break;
			case 5: // out
				output = (get_combo_operand(operand) % 8);
				was_output = true;
				break;
			case 6: // bdv
				combo_operand = get_combo_operand(operand);

				registers[1] = registers[0] >> combo_operand;
				break;
			case 7: // cdv
				combo_operand = get_combo_operand(operand);

				registers[2] = registers[0] >> combo_operand;
				break;
		}

		return false;
	} else {
		return true;
	}
}

std::vector<uint8_t> AoC2024_day17::run_program() {
	bool was_output;
	uint8_t output;
	std::vector<uint8_t> result;

	ip_ = 0;
	result.clear();

	while (!get_program_step(ip_, registers_, was_output, output)) {
		if (was_output) {
			result.push_back(output);
		}
	}

	return result;
}

std::string AoC2024_day17::get_program_output() {
	std::vector<uint8_t> output;
	std::string result = "";

	output = run_program();

	for (auto octal : output) {
		if (!result.empty()) {
			result += ',';
		}

		result.append(std::to_string(octal));
	}

	return result;
}

uint64_t AoC2024_day17::get_a_register_init_value() {
	uint64_t init, inc, shift;
	std::vector<uint8_t> output;

	output.clear();
	init = 1;

	while (output.size() != program_.size()) {
		init = init << 1;
		registers_[0] = init;
		output = run_program();
	}

	while (true) {
		if (output == program_) {
			break;
		}

		for (int i = output.size() - 1; i >= 0; i--) {
			if (output[i] != program_[i]) {
				inc = 1;
				shift = 3 * i;
				inc = inc << shift;
				init += inc;
				break;
			}
		}
		registers_[0] = init;
		output = run_program();
	}

	return init;
}

int32_t AoC2024_day17::get_aoc_day() {
	return 17;
}

int32_t AoC2024_day17::get_aoc_year() {
	return 2024;
}

void AoC2024_day17::tests() {
	std::string result;

	if (init({"Register A: 0", "Register B: 0", "Register C: 9", "", "Program: 2,6"})) {
		result = get_program_output(); // B = 1
	}

	if (init({"Register A: 10", "Register B: 0", "Register C: 0", "", "Program: 5,0,5,1,5,4"})) {
		result = get_program_output(); // 0,1,2
	}

	if (init({"Register A: 2024", "Register B: 0", "Register C: 0", "", "Program: 0,1,5,4,3,0"})) {
		result = get_program_output(); // 4,2,5,6,7,7,7,7,3,1,0
	}

	if (init({"Register A: 0", "Register B: 29", "Register C: 0", "", "Program: 1,7"})) {
		result = get_program_output(); // B = 26
	}

	if (init({"Register A: 0", "Register B: 2024", "Register C: 43690", "", "Program: 4,0"})) {
		result = get_program_output(); // B = 44354
	}

	if (init({"Register A: 729", "Register B: 0", "Register C: 0", "", "Program: 0,1,5,4,3,0"})) {
		result = get_program_output(); // 4,6,3,5,6,3,5,2,1,0
	}

	if (init({"Register A: 2024", "Register B: 0", "Register C: 0", "", "Program: 0,3,5,4,3,0"})) {
		result = get_a_register_init_value(); // 117440
	}
}

bool AoC2024_day17::part1() {
	result1_ = get_program_output();

	return true;
}

bool AoC2024_day17::part2() {
	int64_t result = 0;

	result = get_a_register_init_value();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2024_day17 day17;

	return day17.main_execution();
}
