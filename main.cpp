#include "profiler.h"
#include "program.h"
#include "machine.h"

#include <chrono>
#include <compiler.h>

using namespace std::chrono;

// 15.1s
// 5.7
int main() {
	Program program = Program::load("/Users/elvir/CLionProjects/BrainfuckJIT/fractal.rom");
	Machine machine{};

	Compiler compiler{program};
	const std::vector<BfOp> &vector = compiler.compile();
	BEGIN_PROFILER
	machine.run(vector);
	END_PROFILER
	return 0;
}
