#include "profiler.h"
#include "program.h"
#include "machine.h"

#include <chrono>
#include <compiler.h>
#include <jit_compiler.h>
#include <iomanip>
#include <jit_program.h>
#include <asmjit_compiler.h>
#include <asm_jit_compiler2.h>

using namespace std::chrono;
int main(int argc, char** argv) {
    std::string programPath = argv[1];
	Program program = Program::load(programPath);
	Machine machine{};

	Compiler compiler{program};
	JitCompiler jitCompiler{};
	const std::vector<BfOp> &vector = compiler.compile();
	std::vector<u8> code = jitCompiler.emit(program);
	jit1::JitProgram jitProgram{code};
	jit2::ASMJitCompiler2 asmJitCompiler{};
	BEGIN_PROFILER
//	jitProgram.runBytecode();
	asmJitCompiler.emitAndCall(vector);
	END_PROFILER
	return 0;
}

// 15.1s
// 5.7
int _main() {
	Program program = Program::load("/home/elvircrn/CLionProjects/BrainfuckJIT/fractal.rom");
	Machine machine{};

	Compiler compiler{program};
	JitCompiler jitCompiler{};
	const std::vector<BfOp> &vector = compiler.compile();
	std::vector<u8> code = jitCompiler.emit(program);
	jit1::JitProgram jitProgram{code};
	ASMJitCompiler asmJitCompiler{};
	jitProgram.runBytecode();
	BEGIN_PROFILER
	Machine{}.run(vector);
//	asmJitCompiler.emitAndCall(program);
	END_PROFILER
	return 0;
}
