#ifndef BRAINFUCKJIT_ASM_JIT_COMPILER2_H
#define BRAINFUCKJIT_ASM_JIT_COMPILER2_H

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
#include "bfop.h"

namespace jit2 {

void myputchar(uint8_t c) {
  putchar(c);
}

class ASMJitCompiler2 {
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

  void emitAndCall(const std::vector<BfOp>& program) {
    asmjit::JitRuntime jit_runtime;
    asmjit::CodeHolder code;
    code.init(jit_runtime.codeInfo());
    asmjit::x86::Assembler assm(&code);

    asmjit::x86::Gp dataptr = asmjit::x86::r13;
    assm.mov(dataptr, asmjit::x86::rdi);
    std::stack<std::pair<asmjit::Label, asmjit::Label>> addressStack;

    for (size_t pc{}, ptr{}; pc < program.size(); pc++) {
      const BfOp &op = program[pc];
      switch (op.kind) {
      case BfOpKind::INC_PTR: {
        assm.adc(dataptr, op.argument);
      }
        break;
      case BfOpKind::DEC_PTR: {
        assm.sub(dataptr, op.argument);
      }
        break;
      case BfOpKind::INC_DATA: {
        assm.adc(asmjit::x86::byte_ptr(dataptr), op.argument);
      }
        break;
      case BfOpKind::DEC_DATA: {
        assm.sub(asmjit::x86::byte_ptr(dataptr), op.argument);
      }
        break;
      case BfOpKind::WRITE_STDOUT: {
        assm.sub(asmjit::x86::rsp, 8);
        assm.movzx(asmjit::x86::rdi, asmjit::x86::byte_ptr(dataptr));
        assm.call(asmjit::imm(myputchar));
      }
        break;
      case BfOpKind::LOOP_SET_TO_ZERO: {
        assm.mov(asmjit::x86::byte_ptr(dataptr), 0);
      }
        break;
      case BfOpKind::JUMP_IF_DATA_ZERO: {
        assm.cmp(asmjit::x86::byte_ptr(dataptr), 0);
        assm.comment("Jump if data zero", std::string{"Jump if data zero"}.length());
        asmjit::Label openLabel = assm.newLabel();
        asmjit::Label closeLabel = assm.newLabel();
        assm.jz(closeLabel);
        assm.bind(openLabel);
        addressStack.emplace(openLabel, closeLabel);
      }
        break;
      case BfOpKind::JUMP_IF_DATA_NOT_ZERO: {
        assm.cmp(asmjit::x86::byte_ptr(dataptr), 0);
        assm.comment("Jump if data not zero", std::string{"Jump if data not zero"}.length());
        auto top = addressStack.top();
        addressStack.pop();
        assm.jnz(top.first);
        assm.bind(top.second);
      }
        break;
      case BfOpKind::LOOP_MOVE_DATA: {
        tape[ptr + op.argument] += tape[ptr];
        tape[ptr] = 0;
      }
        break;
      case BfOpKind::LOOP_MOVE_PTR: {
        while (tape[ptr]) {
          ptr += op.argument;
        }
      }
        break;
      default: break;
      }
    }

    assm.leave();

    assm.ret();

    asmjit::CodeBuffer &buf = code.sectionById(0)->buffer();

    std::vector<uint8_t> emitted_code(buf.size());
    memcpy(emitted_code.data(), buf.data(), buf.size());

    using JittedFunc = void (*)(u64);
    JittedFunc func;
    jit_runtime.add(&func, &code);


    const char* filename = "/tmp/bjout.bin";
    FILE* outfile = fopen(filename, "wb");
    if (outfile) {
      size_t n = emitted_code.size();
      if (fwrite(emitted_code.data(), 1, n, outfile) == n) {
        std::cout << "* emitted code to " << filename << "\n";
      }
      fclose(outfile);
    }

    std::cout << "* Memory nonzero locations:\n";

    for (size_t i = 0, pcount = 0; i < tape.size(); ++i) {
      if (tape[i]) {
        std::cout << std::right << "[" << std::setw(3) << i
                  << "] = " << std::setw(3) << std::left
                  << static_cast<int32_t>(tape[i]) << "      ";
        pcount++;

        if (pcount > 0 && pcount % 4 == 0) {
          std::cout << "\n";
        }
      }
    }
    std::cout << "\n";

    func((u64) tape.data());
  }
};
}

#endif //BRAINFUCKJIT_ASM_JIT_COMPILER2_H
