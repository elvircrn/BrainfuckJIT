#ifndef BRAINFUCKJIT_PROGRAM_H
#define BRAINFUCKJIT_PROGRAM_H

#include <string>
#include <vector>
#include <stack>
#include <fstream>

#include "core.h"



class Program {
public:
	std::string program;
	std::vector<u64> jumpTable;

	void calculateJumpTable() {
		std::stack<u64> s;

		for (u64 i = 0; i < program.size(); i++) {
			if (program[i] == '[') {
				s.push(i);
			} else if (program[i] == ']') {
				u64 prevOpen = s.top();
				s.pop();
				jumpTable[i] = prevOpen;
				jumpTable[prevOpen] = i;
			}
		}
	}

	explicit Program(const std::string &_program) : program(_program), jumpTable(_program.length()) {
		calculateJumpTable();
	}

	static Program load(const std::string &path) {
		std::ifstream fin;
		fin.open(path, std::ios::in);
		std::string program;
		char c{};
		while (!fin.eof()) {
			fin.get(c);
			if (c == '>' || c == '<' || c == '+' || c == '-' || c == '.' ||
					c == ',' || c == '[' || c == ']') {
				program += c;
			}
		}
		return Program{program};
	}
};

#endif //BRAINFUCKJIT_PROGRAM_H
