#pragma once

#include "scheme/builtins.h"

std::string Itos(int32_t var);

std::string PrintObj(ObjPtr res);

std::string PrintList(CellPtr cell);
