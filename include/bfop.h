#ifndef BRAINFUCKJIT_BFOP_H
#define BRAINFUCKJIT_BFOP_H

#include "core.h"

enum class BfOpKind {
	INVALID_OP = 0,
	INC_PTR,
	DEC_PTR,
	INC_DATA,
	DEC_DATA,
	READ_STDIN,
	WRITE_STDOUT,
	JUMP_IF_DATA_ZERO,
	JUMP_IF_DATA_NOT_ZERO,
	LOOP_SET_TO_ZERO,
	LOOP_MOVE_PTR,
	LOOP_MOVE_DATA
};

struct BfOp {
	BfOpKind kind;
	s64 argument;

	BfOp(BfOpKind _kind, s64 _argument) : kind(_kind), argument(_argument) { }
};

#endif //BRAINFUCKJIT_BFOP_H
