#ifndef BRAINFUCKJIT_COMPILER_H
#define BRAINFUCKJIT_COMPILER_H

#include <utility>

#include "program.h"
#include "bfop.h"
class Compiler {
	Program program;

	bool matches(const std::vector<BfOp> &program, const std::vector<BfOp> &pattern, const std::vector<bool> &checkArgs) {
		if (program.size() < pattern.size()) {
			return false;
		}

		for (size_t i = 0, j = program.size() - pattern.size(); i < pattern.size(); i++, j++) {
			if (pattern[i].kind != program[j].kind || (checkArgs[i] && pattern[i].argument != program[j].argument)) {
				return false;
			}
		}

		return true;
	}

public:
	explicit Compiler(Program _program) : program(std::move(_program)) {}

	std::vector<BfOp> compile() {
		std::vector<BfOp> ops;

		BfOp currentOp{BfOpKind::INVALID_OP, 0};

		char prev{};
		s64 argument{};
		std::stack<u64> s;
		for (const auto &c : program.program) {
			if (prev && c != prev) {
				if (prev == '<') {
					ops.emplace_back(BfOpKind::DEC_PTR, argument);
				} else if (prev == '>') {
					ops.emplace_back(BfOpKind::INC_PTR, argument);
				} else if (prev == '+') {
					ops.emplace_back(BfOpKind::INC_DATA, argument);
				} else if (prev == '-') {
					ops.emplace_back(BfOpKind::DEC_DATA, argument);
				}
				argument = 0;
			}

			if (c == ',') {
				ops.emplace_back(BfOpKind::READ_STDIN, argument);
			} else if (c == '.') {
				ops.emplace_back(BfOpKind::WRITE_STDOUT, argument);
			} else if (c == '[') {
				s.emplace(ops.size());
				ops.emplace_back(BfOpKind::JUMP_IF_DATA_ZERO, argument);
			} else if (c == ']') {
				ops[s.top()].argument = ops.size();
				ops.emplace_back(BfOpKind::JUMP_IF_DATA_NOT_ZERO, s.top());
				s.pop();
				optimize(ops);
			}

			prev = c;
			argument++;
		}

		return ops;
	}
	void optimize(std::vector<BfOp> &ops) {
	  /*
		// [>>>]
		if (matches(ops, {BfOp(BfOpKind::JUMP_IF_DATA_ZERO, 1),
											BfOp(BfOpKind::INC_PTR, 1),
											BfOp(BfOpKind::JUMP_IF_DATA_NOT_ZERO, 1)},
								{true, false, true})) {
			s64 offset = ops[ops.size() - 2].argument;
			for (size_t i = 0; i < 3; i++) {
				ops.pop_back();
			}
			ops.emplace_back(BfOpKind::LOOP_MOVE_PTR, offset);
		}

		// [<<<]
		if (matches(ops, {BfOp(BfOpKind::JUMP_IF_DATA_ZERO, 1),
											BfOp(BfOpKind::INC_PTR, 1),
											BfOp(BfOpKind::JUMP_IF_DATA_NOT_ZERO, 1)},
								{true, false, true})) {
			s64 offset = -ops[ops.size() - 2].argument;
			for (size_t i = 0; i < 3; i++) {
				ops.pop_back();
			}
			ops.emplace_back(BfOpKind::LOOP_MOVE_PTR, offset);
		}

		// [-<+>]
		if (matches(ops, {BfOp(BfOpKind::JUMP_IF_DATA_ZERO, 0),
											BfOp(BfOpKind::DEC_DATA, 1),
											BfOp(BfOpKind::DEC_PTR, 0),
											BfOp(BfOpKind::INC_DATA, 1),
											BfOp(BfOpKind::INC_PTR, 1),
											BfOp(BfOpKind::JUMP_IF_DATA_NOT_ZERO, 1)},
								{false, true, false, true, false, false}) &&
				ops[ops.size() - 2].argument == ops[ops.size() - 4].argument) {
			s64 offset = -ops[ops.size() - 2].argument;
			for (size_t i = 0; i < 6; i++) {
				ops.pop_back();
			}
			ops.emplace_back(BfOpKind::LOOP_MOVE_DATA, offset);
		}


		// [->+<]
		if (matches(ops, {BfOp(BfOpKind::JUMP_IF_DATA_ZERO, 0),
											BfOp(BfOpKind::DEC_DATA, 1),
											BfOp(BfOpKind::INC_PTR, 0),
											BfOp(BfOpKind::INC_DATA, 1),
											BfOp(BfOpKind::DEC_PTR, 1),
											BfOp(BfOpKind::JUMP_IF_DATA_NOT_ZERO, 1)},
								{false, true, false, true, false, false}) &&
				ops[ops.size() - 2].argument == ops[ops.size() - 4].argument) {
			s64 offset = ops[ops.size() - 2].argument;
			for (size_t i = 0; i < 6; i++) {
				ops.pop_back();
			}
			ops.emplace_back(BfOpKind::LOOP_MOVE_DATA, offset);
		}
*/
		// [-]
		if (matches(ops, {BfOp(BfOpKind::JUMP_IF_DATA_ZERO, 1),
											BfOp(BfOpKind::DEC_DATA, 1),
											BfOp(BfOpKind::JUMP_IF_DATA_NOT_ZERO, 1)},
								{true, true, true})) {
			for (size_t i = 0; i < 3; i++) {
				ops.pop_back();
			}
			ops.emplace_back(BfOpKind::LOOP_SET_TO_ZERO, 0);
		}
	}
};

#endif //BRAINFUCKJIT_COMPILER_H
