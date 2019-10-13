#ifndef BRAINFUCKJIT_JIT_COMPILER_H
#define BRAINFUCKJIT_JIT_COMPILER_H

#include <array>
#include <sys/mman.h>
#include <cstring>

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
// Computes a 32-bit relative offset for pc-relative jumps. Given an address to
// jump *from* and an address to jump *to*, returns the 4-byte integer that has
// to be encoded into the offset field of a jmp instruction. The user is
// expected to adjust jump addresses before passing them to this function (for
// example taking into account that a jump offset is computed from after the
// jump instruction itself).
// Note: this is very specific to the x64 architecture.
	u32 computeRelative32bitOffset(u32 jump_from, u32 jump_to) {
		if (jump_from <= jump_to) {
			return jump_to - jump_from;
		} else {
			return ~(jump_from - jump_to) + 1;
		}
	}

	std::vector<u8> emit(const Program &program) {
		CodeEmitter codeEmitter;

		size_t pc{};
		std::stack<u32> addressStack;

		codeEmitter.emitU8({0x49, 0xBD});
		codeEmitter.emitU64((u64) tape.data());


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
				// mov rax, 0x2000004     ; sys_write call identifier
				// mov rdi, 1             ; STDOUT file descriptor
				// mov rsi, (%r13) ; buffer to print
				// mov rdx, 1 ; length of buffer
				// syscall                ; make the system call
			} else if (c == '.') {
				codeEmitter.emitU8({ 0x48, 0xC7, 0xC0, 0x04, 0x00, 0x00, 0x02, 0x48, 0xC7, 0xC7, 0x01, 0x00, 0x00, 0x00, 0x4C, 0x89, 0xEE, 0x48, 0xC7, 0xC2, 0x01, 0x00, 0x00, 0x00, 0x0F, 0x05 });
			} else if (c == ',') {
				codeEmitter.emitU8({ 0x48, 0xC7, 0xC0, 0x04, 0x00, 0x00, 0x02, 0x48, 0xC7, 0xC7, 0x01, 0x00, 0x00, 0x00, 0x4C, 0x89, 0xEE, 0x48, 0xC7, 0xC2, 0x01, 0x00, 0x00, 0x00, 0x0F, 0x05 });
			} else if (c == '[') {
				// cmp BYTE PTR [%r13], 0
				codeEmitter.emitU8({0x41, 0x80, 0x7D, 0x00, 0x00});
				addressStack.push(codeEmitter.code.size());

				// jz short $0000
				codeEmitter.emitU8({0x0F, 0x84});
				codeEmitter.emitU32(0);
			} else if (c == ']') {
				u32 openBracketOffset = addressStack.top();
				addressStack.pop();

				// cmp BYTE PTR [%r13], 0
				codeEmitter.emitU8({0x41, 0x80, 0x7D, 0x00, 0x00});

				u32 jumpBackTo = openBracketOffset + 6;
				u32 jumpBackFrom = codeEmitter.code.size() + 6;

				u32 backOffset = computeRelative32bitOffset(jumpBackFrom, jumpBackTo);

				// jnz short $0000
				codeEmitter.emitU8({0x0F, 0x85});
				codeEmitter.emitU32(backOffset);

				u32 jumpForwardFrom = openBracketOffset + 6;
				u32 jumpForwardTo = codeEmitter.code.size();
				u32 forwardOffset = computeRelative32bitOffset(jumpForwardFrom, jumpForwardTo);

				codeEmitter.replaceU32(openBracketOffset + 2, forwardOffset);
			}
		}

		codeEmitter.emitU8({0xc3});

		return codeEmitter.code;
	}
};

#endif //BRAINFUCKJIT_JIT_COMPILER_H
