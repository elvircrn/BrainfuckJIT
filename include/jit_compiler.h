#ifndef BRAINFUCKJIT_JIT_COMPILER_H
#define BRAINFUCKJIT_JIT_COMPILER_H

#include <array>

#include "program.h"
#include "code_emitter.h"

// Registers used in the program:
//
// r13: the data pointer -- contains the address of memory.data()
//
// rax, rdi, rsi, rdx: used for making system calls, per the ABI.
class JitCompiler {
	std::array<u8, 30000> tape;

public:
	void run(const Program &program) {
		CodeEmitter codeEmitter;

		size_t pc{}, ptr{};

		for (; pc < program.program.length(); pc++) {
			char c = program.program[pc];
			// inc r13
			if (c == '>') {
				codeEmitter.emitU8({0x49u, 0xffu, 0xc5});
			// dec r13
			} else if (c == '<') {
				codeEmitter.emitU8({0x49u, 0xffu, 0xcd});
			// add BYTE PTR [r13+0x0],0x1
			} else if (c == '+') {
				codeEmitter.emitU8({0x41, 0x80, 0x45, 0x00, 0x01});
			// sub BYTE PTR [r13+0x0],0x1
			} else if (c == '-') {
				codeEmitter.emitU8({0x41, 0x80, 0x6d, 0x00, 0x01});
			// To emit one byte to stdout, call the write syscall with fd=1 (for
			// stdout), buf=address of byte, count=1.
			//
			// mov $1, %rax
			// mov $1, %rdi
			// mov %r13, %rsi
			// mov $1, %rdx
			// syscall
			} else if (c == '.') {
				codeEmitter.emitU8({0x48, 0xC7, 0xC0, 0x01, 0x00, 0x00, 0x00});
				codeEmitter.emitU8({0x48, 0xC7, 0xC7, 0x01, 0x00, 0x00, 0x00});
				codeEmitter.emitU8({0x4C, 0x89, 0xEE});
				codeEmitter.emitU8({0x48, 0xC7, 0xC2, 0x01, 0x00, 0x00, 0x00});
				codeEmitter.emitU8({0x0F, 0x05});
			} else if (c == ',') {
				// To read one byte from stdin, call the read syscall with fd=0 (for
				// stdin),
				// buf=address of byte, count=1.
				codeEmitter.emitU8({0x48, 0xC7, 0xC0, 0x00, 0x00, 0x00, 0x00});
				codeEmitter.emitU8({0x48, 0xC7, 0xC7, 0x00, 0x00, 0x00, 0x00});
				codeEmitter.emitU8({0x4C, 0x89, 0xEE});
				codeEmitter.emitU8({0x48, 0xC7, 0xC2, 0x01, 0x00, 0x00, 0x00});
				codeEmitter.emitU8({0x0F, 0x05});
			} else if (c == '[') {
			} else if (c == ']') {
			}
		}
	}
};

#endif //BRAINFUCKJIT_JIT_COMPILER_H
