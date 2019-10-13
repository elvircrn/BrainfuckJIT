#ifndef BRAINFUCKJIT_PROGRAM_H
#define BRAINFUCKJIT_PROGRAM_H

#include <string>
#include <utility>
#include <vector>
#include <stack>
#include <fstream>

#include "core.h"

class Program {
public:
  std::string program;

  explicit Program(std::string _program) : program(std::move(_program)) {}

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
