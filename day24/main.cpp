#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
#include <map>
#include <queue>
#include <set>

enum operation_t { E_OPERATION_AND, E_OPERATION_OR, E_OPERATION_XOR };

struct operation_str {
	std::string output;
	std::string input1;
	std::string input2;
	operation_t operation;

	void print() const {
		std::cout << input1
				  << (operation == E_OPERATION_AND	? " AND "
					  : operation == E_OPERATION_OR ? " OR "
													: " XOR ")
				  << input2 << " = " << output << std::endl;
	}

	bool isXYoperation() const {
		return (input1[0] == 'x' && input2[0] == 'y') || (input1[0] == 'y' && input2[0] == 'x');
	}

	bool has_input(const std::string& var) const {
		return (input1 == var || input2 == var);
	}
};

struct wiring_str {
	uint64_t bit;
	uint64_t input;
	uint64_t outputx;
	uint64_t outputy;
	uint64_t outputxy;
	std::string xname;
	std::string yname;
	std::string zname;

	bool is_valid_single(const bool addition) const {
		if (addition) {
			return (outputx == input) && (outputy == input);
		} else {
			return (outputx == 0) && (outputy == 0);
		}
	}

	bool is_valid_couple(const bool addition) const {
		if (addition) {
			return (outputxy == input * 2);
		} else {
			return (outputxy == input);
		}
	}

	bool is_valid(const bool addition) const {
		return is_valid_single(addition) && is_valid_couple(addition);
	}

	void print() const {
		std::cout << "Wiring: " << std::endl;
		std::cout << "  Input: " << input << " [" << bit << "]" << std::endl;
		std::cout << "  Output X: " << outputx << std::endl;
		std::cout << "  Output Y: " << outputy << std::endl;
		std::cout << "  Output XY: " << outputxy << std::endl;
		std::cout << "  Names: " << xname << ", " << yname << ", " << zname << std::endl;
	}
};

class AoC2024_day24 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::map<std::string, bool> states_;
	std::map<std::string, operation_str> operations_;
	std::map<uint8_t, std::string> xorsXY_, andsXY_, orsXY_;
	std::map<std::string, std::set<std::string>> others_;
	uint64_t get_decimal_output_number(std::map<std::string, bool>& states_init);
	std::string get_swap_wires_list(bool addition);
	void check_wiring(std::map<std::string, bool>& states, wiring_str& wiring);
	void swap_operations(const std::string swap_op1, const std::string swap_op2);

  public:
};

bool AoC2024_day24::init(const std::vector<std::string> lines) {
	bool states = true;
	std::vector<std::string> parts;

	operations_.clear();
	states_.clear();
	xorsXY_.clear();
	andsXY_.clear();
	orsXY_.clear();
	others_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (states) {
			if (lines[i].empty()) {
				states = false;
				continue;
			}

			parts = split(lines[i], ": ");
			if (parts.size() != 2) {
				std::cout << "Invalid state definition at line " << i + 1 << std::endl;
				return false;
			}

			states_[parts[0]] = (parts[1] == "1");
		} else {
			parts = split(lines[i], " ");
			if (parts.size() != 5) {
				std::cout << "Invalid operation definition at line " << i + 1 << std::endl;
				return false;
			}

			if (parts[3] != "->") {
				std::cout << "Invalid operation format at line " << i + 1 << std::endl;
				return false;
			}

			operation_str op;
			op.input1 = parts[0];
			op.input2 = parts[2];
			op.output = parts[4];

			if (parts[1] == "AND") {
				op.operation = E_OPERATION_AND;

				if (op.isXYoperation()) {
					andsXY_[std::stoi(op.input1.substr(1))] = op.output;
				}
			} else if (parts[1] == "OR") {
				op.operation = E_OPERATION_OR;

				if (op.isXYoperation()) {
					orsXY_[std::stoi(op.input1.substr(1))] = op.output;
				}
			} else if (parts[1] == "XOR") {
				op.operation = E_OPERATION_XOR;

				if (op.isXYoperation()) {
					xorsXY_[std::stoi(op.input1.substr(1))] = op.output;
				}
			} else {
				std::cout << "Invalid operation at line " << i + 1 << std::endl;
				return false;
			}

			operations_[parts[4]] = op;

			others_[op.input1].emplace(op.output);
			others_[op.input2].emplace(op.output);
		}
	}

	return true;
}

uint64_t AoC2024_day24::get_decimal_output_number(std::map<std::string, bool>& states_init) {
	uint64_t result = 0;
	std::map<std::string, bool> states = states_init;
	std::map<std::string, operation_str> operations = operations_;
	bool allZdone = false;
	uint8_t loop = 0;

	while (!allZdone && (loop < 5)) {
		allZdone = true;

		for (const auto& [output, operation] : operations) {
			if (states.count(operation.input1) && states.count(operation.input2)) {

				loop = 0;

				switch (operation.operation) {
					case E_OPERATION_AND:
						states[output] = states[operation.input1] && states[operation.input2];
						break;
					case E_OPERATION_OR:
						states[output] = states[operation.input1] || states[operation.input2];
						break;
					case E_OPERATION_XOR:
						states[output] = states[operation.input1] ^ states[operation.input2];
						break;
				}

				operations.erase(output);
				allZdone = false;
				break;
			} else {
				if (output[0] == 'z') {
					allZdone = false;
				}
			}
		}
		loop++;
	}

	if (loop >= 5) {
		return UINT64_MAX;
	}

	for (const auto& [key, value] : states) {
		if (key[0] == 'z') {
			if (value) {
				uint64_t mask = 1ULL << (std::stoi(key.substr(1)));

				result |= mask;
			}
		}
	}

	return result;
}

void AoC2024_day24::swap_operations(const std::string swap_op1, const std::string swap_op2) {
	operation_str op1 = operations_[swap_op1];
	operation_str op2 = operations_[swap_op2];

	std::swap(op1.output, op2.output);

	operations_[swap_op1] = op2;
	operations_[swap_op2] = op1;
}

void AoC2024_day24::check_wiring(std::map<std::string, bool>& states, wiring_str& wiring) {
	states[wiring.xname] = true;
	wiring.outputx = get_decimal_output_number(states);
	states[wiring.xname] = false;

	states[wiring.yname] = true;
	wiring.outputy = get_decimal_output_number(states);
	states[wiring.yname] = false;

	states[wiring.xname] = true;
	states[wiring.yname] = true;
	wiring.outputxy = get_decimal_output_number(states);
	states[wiring.xname] = false;
	states[wiring.yname] = false;
}

std::string AoC2024_day24::get_swap_wires_list(bool addition) {
	// swap wirings detection not universal, made just for this input
	std::set<std::string> swap_wires;
	std::string result;
	std::map<uint8_t, std::string> xnames, ynames;
	uint8_t input_size = 0;
	std::map<std::string, bool> states;
	std::string tmp, number;
	uint64_t op_input;
	wiring_str wiring;

	states.clear();

	while (1) {
		number = std::to_string(input_size);
		if (number.size() == 1) {
			number = "0" + number; // Ensure two-digit format
		}

		tmp = "x" + number;

		if (states_.count(tmp)) {
			xnames[input_size] = tmp;
		} else {
			input_size--;
			break;
		}

		tmp = "y" + number;

		if (states_.count(tmp)) {
			ynames[input_size] = tmp;
		} else {
			input_size--;
			break;
		}

		states[xnames[input_size]] = false;
		states[ynames[input_size]] = false;
		input_size++;
	}

	for (size_t i = 0; i <= input_size; i++) {
		op_input = 1;
		op_input = op_input << i;

		wiring.input = op_input;
		wiring.bit = i;
		wiring.xname = xnames[i];
		wiring.yname = ynames[i];
		wiring.zname = xnames[i];
		wiring.zname[0] = 'z'; // Change first character to 'z' for output

		check_wiring(states, wiring);

		if (!wiring.is_valid(addition)) {
			if (addition) {
				// wiring.print();
				std::string x_and_y, x_xor_y, x_xor_y_xor, x_xor_y_and;
				std::set<std::string> xy_xors = {};

				if (xorsXY_.count(i) && andsXY_.count(i)) {
					x_xor_y = xorsXY_[i];
					x_and_y = andsXY_[i];

					xy_xors = others_[x_xor_y];

					if (xy_xors.size() < 2) {
						swap_wires.emplace(x_xor_y);
						swap_wires.emplace(x_and_y);
					} else {
						for (const auto& out : xy_xors) {
							if (operations_[out].input1 == x_xor_y) {
								if (operations_[out].operation == E_OPERATION_XOR) {
									x_xor_y_xor = out;
								} else if (operations_[out].operation == E_OPERATION_AND) {
									x_xor_y_and = out;
								}
							} else if (operations_[out].input2 == x_xor_y) {
								if (operations_[out].operation == E_OPERATION_XOR) {
									x_xor_y_xor = out;
								} else if (operations_[out].operation == E_OPERATION_AND) {
									x_xor_y_and = out;
								}
							}
						}

						if (x_xor_y_xor != wiring.zname) {
							swap_wires.emplace(x_xor_y_xor);
							swap_wires.emplace(wiring.zname);
						}
					}
				}
			} else {
				swap_wires.emplace(wiring.zname);
			}
		}
	}

	result = join(swap_wires, ",");

	return result;
}

int32_t AoC2024_day24::get_aoc_day() {
	return 24;
}

int32_t AoC2024_day24::get_aoc_year() {
	return 2024;
}

void AoC2024_day24::tests() {
	uint64_t result;
	std::string result2;

	if (init({"x00: 1", "x01: 1", "x02: 1", "y00: 0", "y01: 1", "y02: 0", "", "x00 AND y00 -> z00", "x01 XOR y01 -> z01", "x02 OR y02 -> z02"})) {
		result = get_decimal_output_number(states_); // 4
	}

	if (init({"x00: 1",
			  "x01: 0",
			  "x02: 1",
			  "x03: 1",
			  "x04: 0",
			  "y00: 1",
			  "y01: 1",
			  "y02: 1",
			  "y03: 1",
			  "y04: 1",
			  "",
			  "ntg XOR fgs -> mjb",
			  "y02 OR x01 -> tnw",
			  "kwq OR kpj -> z05",
			  "x00 OR x03 -> fst",
			  "tgd XOR rvg -> z01",
			  "vdt OR tnw -> bfw",
			  "bfw AND frj -> z10",
			  "ffh OR nrd -> bqk",
			  "y00 AND y03 -> djm",
			  "y03 OR y00 -> psh",
			  "bqk OR frj -> z08",
			  "tnw OR fst -> frj",
			  "gnj AND tgd -> z11",
			  "bfw XOR mjb -> z00",
			  "x03 OR x00 -> vdt",
			  "gnj AND wpb -> z02",
			  "x04 AND y00 -> kjc",
			  "djm OR pbm -> qhw",
			  "nrd AND vdt -> hwm",
			  "kjc AND fst -> rvg",
			  "y04 OR y02 -> fgs",
			  "y01 AND x02 -> pbm",
			  "ntg OR kjc -> kwq",
			  "psh XOR fgs -> tgd",
			  "qhw XOR tgd -> z09",
			  "pbm OR djm -> kpj",
			  "x03 XOR y03 -> ffh",
			  "x00 XOR y04 -> ntg",
			  "bfw OR bqk -> z06",
			  "nrd XOR fgs -> wpb",
			  "frj XOR qhw -> z04",
			  "bqk OR frj -> z07",
			  "y03 OR x01 -> nrd",
			  "hwm AND bqk -> z03",
			  "tgd XOR rvg -> z12",
			  "tnw OR pbm -> gnj"})) {
		result = get_decimal_output_number(states_); // 2024
	}

	if (init({"x00: 0", "x01: 1", "x02: 0", "x03: 1", "x04: 0", "x05: 1", "y00: 0", "y01: 0", "y02: 1", "y03: 1", "y04: 0", "y05: 1", "", "x00 AND y00 -> z05",
			  "x01 AND y01 -> z02", "x02 AND y02 -> z01", "x03 AND y03 -> z03", "x04 AND y04 -> z04", "x05 AND y05 -> z00"})) {
		result2 = get_swap_wires_list(false); // "z00,z01,z02,z05"
	}
}

bool AoC2024_day24::part1() {
	uint64_t result = 0;

	result = get_decimal_output_number(states_);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2024_day24::part2() {
	result2_ = get_swap_wires_list(true);

	return true;
}

int main(void) {
	AoC2024_day24 day24;

	return day24.main_execution();
}
