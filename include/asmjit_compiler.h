#ifndef BRAINFUCKJIT_ASMJIT_COMPILER_H
#define BRAINFUCKJIT_ASMJIT_COMPILER_H

#include <array>
#include <utility>
#include <sys/mman.h>
#include <cstring>
#include <asmjit.h>
#include <x86/x86assembler.h>
#include <x86/x86operand.h>
#include <x86/x86emitter.h>

#include "program.h"
#include "code_emitter.h"

void myputchar(uint8_t c) {
	putchar(c);
}

class ASMJitCompiler {
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

	void emitAndCall(const Program &program) {
		asmjit::JitRuntime jit_runtime;
		asmjit::CodeHolder code;
		code.init(jit_runtime.codeInfo());
		asmjit::x86::Assembler assm(&code);
		size_t pc{};

		asmjit::x86::Gp dataptr = asmjit::x86::r13;
		assm.mov(dataptr, asmjit::x86::rdi);
		std::stack<std::pair<asmjit::Label, asmjit::Label>> addressStack;

		for (; pc < program.program.length(); pc++) {
			char c = program.program[pc];
			if (c == '>') {
				assm.inc(dataptr);
			} else if (c == '<') {
				assm.dec(dataptr);
			} else if (c == '+') {
				assm.inc(asmjit::x86::byte_ptr(dataptr));
			} else if (c == '-') {
				assm.dec(asmjit::x86::byte_ptr(dataptr));
			} else if (c == '.') {
				assm.sub(asmjit::x86::rsp, 8);
				assm.movzx(asmjit::x86::rdi, asmjit::x86::byte_ptr(dataptr));
				assm.call((u64) myputchar);
			} else if (c == ',') {
			} else if (c == '[') {
				assm.cmp(asmjit::x86::byte_ptr(dataptr), 0);
				asmjit::Label openLabel = assm.newLabel();
				asmjit::Label closeLabel = assm.newLabel();
				assm.jz(closeLabel);
				assm.bind(openLabel);
				addressStack.emplace(openLabel, closeLabel);
			} else if (c == ']') {
				assm.cmp(asmjit::x86::byte_ptr(dataptr), 0);
				auto top = addressStack.top();
				addressStack.pop();
				assm.jnz(top.first);
				assm.bind(top.second);
			}
		}

		assm.ret();

		asmjit::CodeBuffer& buf = code.sectionById(0)->buffer();

		std::vector<uint8_t> emitted_code(buf.size());
		memcpy(emitted_code.data(), buf.data(), buf.size());

		using JittedFunc = void (*)(u64);
		JittedFunc func;
		jit_runtime.add(&func, &code);

		func((u64) tape.data());
	}
};

#endif //BRAINFUCKJIT_ASMJIT_COMPILER_H
