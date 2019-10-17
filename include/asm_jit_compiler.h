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
#include "bfop.h"

namespace jit {

template<int TAPE_SIZE = 30000>
class BrainfuckJITRuntime {
  std::array<u8, TAPE_SIZE> tape;

public:
  void emitAndCall(const std::vector<BfOp> &program) {
    asmjit::JitRuntime jitRuntime;
    asmjit::CodeHolder code;
    code.init(jitRuntime.codeInfo());
    asmjit::x86::Assembler assm(&code);
    emit(program, assm);
    run(jitRuntime, code);
  }

  void run(asmjit::JitRuntime &jit_runtime, asmjit::CodeHolder &code) {
    asmjit::CodeBuffer &buf = code.sectionById(0)->buffer();
    std::vector<uint8_t> emitted_code(buf.size());
    memcpy(emitted_code.data(), buf.data(), buf.size());
      const char* filename = "/tmp/bjoutO5.bin";
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

    using JittedFunc = void (*)(u64);
    JittedFunc func;
    jit_runtime.add(&func, &code);
    func((u64) tape.data());
  }

  void emitFuncReturn(asmjit::x86::Assembler &assm) {
    // TODO: Figure out why this is necessary
    assm.leave();
    assm.ret();
  }

  void emit(const std::vector<BfOp> &program, asmjit::x86::Assembler &assm) {
    asmjit::x86::Gp dataptr = asmjit::x86::r13;
    assm.mov(dataptr, asmjit::x86::rdi);
    std::stack<std::pair<asmjit::Label, asmjit::Label>> addressStack;

    for (const auto &op : program) {
      switch (op.kind) {
      case BfOpKind::INC_PTR:emitIncPtr(assm, dataptr, op);
        break;
      case BfOpKind::DEC_PTR:emitDecPtr(assm, dataptr, op);
        break;
      case BfOpKind::INC_DATA:emitIncData(assm, dataptr, op);
        break;
      case BfOpKind::DEC_DATA:emitDecData(assm, dataptr, op);
        break;
      case BfOpKind::WRITE_STDOUT:emitWriteStdout(assm, dataptr);
        break;
      case BfOpKind::LOOP_SET_TO_ZERO:emitLoopSetToZero(assm, dataptr);
        break;
      case BfOpKind::JUMP_IF_DATA_ZERO:emitJumpIfDataZero(assm, dataptr, addressStack);
        break;
      case BfOpKind::JUMP_IF_DATA_NOT_ZERO:emitJumpIfDataNotZero(assm, dataptr, addressStack);
        break;
      case BfOpKind::LOOP_MOVE_DATA:emitLoopMoveData(assm);
        break;
      case BfOpKind::LOOP_MOVE_PTR:emitLoopMovePtr(assm, dataptr, op);
        break;
      default: break;
      }
    }

    emitFuncReturn(assm);
  }

  inline void emitLoopMoveData(asmjit::x86::Assembler &assm) {}

  inline void emitLoopMovePtr(asmjit::x86::Assembler &assm,
                              const asmjit::x86::Gp &dataptr,
                              const BfOp &op) {
    asmjit::Label startLoop = assm.newLabel();
    asmjit::Label endLoop = assm.newLabel();
    assm.bind(startLoop);
    assm.cmp(asmjit::x86::byte_ptr(dataptr), 0);
    assm.jz(endLoop);
    if (op.argument > 0) {
      assm.adc(asmjit::x86::byte_ptr(dataptr), op.argument);
    } else if (op.argument < 0) {
      assm.sub(asmjit::x86::byte_ptr(dataptr), -op.argument);
    }
    assm.jmp(startLoop);
    assm.bind(endLoop);
  }

  inline void emitJumpIfDataNotZero(asmjit::x86::Assembler &assm,
                                    const asmjit::x86::Gp &dataptr,
                                    std::stack<std::pair<asmjit::Label, asmjit::Label>> &addressStack) const {
    auto top = addressStack.top(); addressStack.pop();
    assm.cmp(asmjit::x86::byte_ptr(dataptr), 0);
    assm.jnz(top.first);
    assm.bind(top.second);
  }

  inline void emitJumpIfDataZero(asmjit::x86::Assembler &assm,
                                 const asmjit::x86::Gp &dataptr,
                                 std::stack<std::pair<asmjit::Label, asmjit::Label>> &addressStack) const {
    assm.cmp(asmjit::x86::byte_ptr(dataptr), 0);
    asmjit::Label openLabel = assm.newLabel();
    asmjit::Label closeLabel = assm.newLabel();
    assm.jz(closeLabel);
    assm.bind(openLabel);
    addressStack.emplace(openLabel, closeLabel);
  }

  inline void emitLoopSetToZero(asmjit::x86::Assembler &assm,
                                const asmjit::x86::Gp &dataptr) const { assm.mov(asmjit::x86::byte_ptr(dataptr), 0); }

  inline void emitWriteStdout(asmjit::x86::Assembler &assm, const asmjit::x86::Gp &dataptr) const {
    assm.sub(asmjit::x86::rsp, 8);
    assm.movzx(asmjit::x86::rdi, asmjit::x86::byte_ptr(dataptr));
    assm.call(asmjit::imm(std::putchar));
  }

  inline void emitDecData(asmjit::x86::Assembler &assm,
                          const asmjit::x86::Gp &dataptr,
                          const BfOp &op) const { assm.sub(asmjit::x86::byte_ptr(dataptr), op.argument); }

  inline void emitIncData(asmjit::x86::Assembler &assm,
                          const asmjit::x86::Gp &dataptr,
                          const BfOp &op) const { assm.adc(asmjit::x86::byte_ptr(dataptr), op.argument); }

  inline void emitDecPtr(asmjit::x86::Assembler &assm,
                         const asmjit::x86::Gp &dataptr,
                         const BfOp &op) const { assm.sub(dataptr, op.argument); }

  inline void emitIncPtr(asmjit::x86::Assembler &assm,
                         const asmjit::x86::Gp &dataptr,
                         const BfOp &op) const { assm.adc(dataptr, op.argument); }
};
}

#endif //BRAINFUCKJIT_ASM_JIT_COMPILER2_H
