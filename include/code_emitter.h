#ifndef BRAINFUCKJIT_CODE_EMITTER_H
#define BRAINFUCKJIT_CODE_EMITTER_H

#include <vector>
#include "core.h"

class CodeEmitter {
public:
	std::vector<u8> code;
	CodeEmitter() = default;

	void emitByte(uint8_t v) {
		code.emplace_back(v);
	}

	// Emits a sequence of consecutive bytes.
	void emitU8(std::initializer_list<uint8_t> seq) {
		for (const auto& v : seq) {
			emitByte(v);
		}
	}

	void emitU32(uint32_t v) {
		emitByte(v & 0xffu);
		emitByte((v >> 8u) & 0xffu);
		emitByte((v >> 16u) & 0xffu);
		emitByte((v >> 24u) & 0xffu);
	}

	void emitU64(uint64_t v) {
		emitU32(v & 0xffffffffu);
		emitU32((v >> 32u) & 0xffffffffu);
	}

	// Replaces the byte at 'offset' with 'v'. Assumes offset < size().
	void replaceU8(size_t offset, u8 v) {
//		assert(offset < code.size() && "replacement doesn't fit in code");
		code[offset] = v;
	}

	// Replaces the 32-bit word at 'offset' with 'v'. Assumes offset + 3 < size().
	void replaceU32(size_t offset, u32 v) {
		replaceU8(offset, v & 0xffu);
		replaceU8(offset + 1, (v >> 8u) & 0xffu);
		replaceU8(offset + 2, (v >> 16u) & 0xffu);
		replaceU8(offset + 3, (v >> 24u) & 0xffu);
	}
};

#endif //BRAINFUCKJIT_CODE_EMITTER_H
