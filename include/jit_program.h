#ifndef BRAINFUCKJIT_JIT_PROGRAM_H
#define BRAINFUCKJIT_JIT_PROGRAM_H

#include <vector>
#include <sys/mman.h>
#include "core.h"

namespace jit1 {

void *alloc_writable_memory(size_t size) {
	void *ptr =
			mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (ptr == (void *) -1) {
		perror("mmap");
		return nullptr;
	}
	return ptr;
}

// Sets a RX permission on the given memory, which must be page-aligned. Returns
// 0 on success. On failure, prints out the error and returns -1.
int make_memory_executable(void *m, size_t size) {
	if (mprotect(m, size, PROT_READ | PROT_EXEC) == -1) {
		perror("mprotect");
		return -1;
	}
	return 0;
}

class JitProgram {
	size_t programSize;
	void *programMemory;

public:

	JitProgram(std::vector<u8> &code) {
		programSize = code.size();
		programMemory = alloc_writable_memory(programSize);
		std::memcpy(programMemory, code.data(), programSize);
		make_memory_executable(programMemory, programSize);
	}

	~JitProgram() {
		if (programMemory != nullptr) {
			if (munmap(programMemory, programSize) < 0) {
				perror("munmap");
			}
		}
	}

	void runBytecode() {
		using JittedFunc = void (*)(void);
		auto jittedFunc = (JittedFunc) programMemory;
		jittedFunc();
	}

};

}
#endif //BRAINFUCKJIT_JIT_PROGRAM_H
