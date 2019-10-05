#ifndef BRAINFUCKJIT_MACHINE_H
#define BRAINFUCKJIT_MACHINE_H

#include <array>
#include <iostream>

#include "core.h"
#include "program.h"
#include "bfop.h"

class Machine {
	static constexpr u64 TAPE_SIZE = 30000;
	std::array<s64, TAPE_SIZE> tape{};

public:
	void run(const std::vector<BfOp> &program) {
		for (size_t pc{}, ptr{}; pc < program.size(); pc++) {
			const BfOp &op = program[pc];
			switch (op.kind) {
			case BfOpKind::INC_DATA: tape[ptr] += op.argument;
				break;
			case BfOpKind::DEC_DATA: tape[ptr] -= op.argument;
				break;
			case BfOpKind::INC_PTR: ptr += op.argument;
				break;
			case BfOpKind::DEC_PTR: ptr -= op.argument;
				break;
			case BfOpKind::WRITE_STDOUT: std::cout.put((char) tape[ptr]);
				break;
			case BfOpKind::LOOP_SET_TO_ZERO: tape[ptr] = 0;
				break;
			case BfOpKind::JUMP_IF_DATA_ZERO:
				if (!tape[ptr]) {
					pc = op.argument;
				}
				break;
			case BfOpKind::JUMP_IF_DATA_NOT_ZERO:
				if (tape[ptr]) {
					pc = op.argument;
				}
				break;
			case BfOpKind::LOOP_MOVE_DATA: tape[ptr + op.argument] += tape[ptr]; tape[ptr] = 0;
				break;
			case BfOpKind::LOOP_MOVE_PTR:
				while (tape[ptr]) {
					ptr += op.argument;
				}
			default: break;
			}
		}
	}

	void run(const Program &program) {
		size_t pc{}, ptr{};

		for (; pc < program.program.length(); pc++) {
			char c = program.program[pc];
			if (c == '>') {
				ptr++;
			} else if (c == '<') {
				ptr--;
			} else if (c == '+') {
				tape[ptr]++;
			} else if (c == '-') {
				tape[ptr]--;
			} else if (c == '.') {
				std::cout.put((char) tape[ptr]);
			} else if (c == ',') {
				tape[ptr] = std::cin.get();
			} else if (c == '[') {
				if (!tape[ptr]) {
					pc = program.jumpTable[pc];
				}
			} else if (c == ']') {
				if (tape[ptr]) {
					pc = program.jumpTable[pc];
				}
			}
		}
	}
};

#endif //BRAINFUCKJIT_MACHINE_H
