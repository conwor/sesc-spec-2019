#pragma once

#include "base.h"

// Bytecode print

void writeBytecode(Bytecode* bc, std::string filename);

void writeBytecode(Bytecode* bc, std::ostream& os);

