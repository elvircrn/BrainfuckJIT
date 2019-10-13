#include "profiler.h"
#include "program.h"

#include <chrono>
#include <compiler.h>
#include <iomanip>
#include <asm_jit_compiler.h>

using namespace std::chrono;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
int main(int argc, char** argv) {
    std::string programPath = argv[1];
	Program program = Program::load(programPath);
	Compiler compiler{};
	const std::vector<BfOp> &compiled = compiler.compile(program);
	jit::BrainfuckJITRuntime brainfuckJITRuntime{};
	BEGIN_PROFILER
	brainfuckJITRuntime.emitAndCall(compiled);
	END_PROFILER
	return 0;
}
#pragma clang diagnostic pop

