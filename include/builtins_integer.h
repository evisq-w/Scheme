#pragma once

#include "scheme/builtins.h"

ObjPtr BuiltinIsNumber(const CellPtr& cell);

ObjPtr BuiltinLess(const CellPtr& cell);

ObjPtr BuiltinLessEq(const CellPtr& cell);

ObjPtr BuiltinGreater(const CellPtr& cell);

ObjPtr BuiltinGreaterEq(const CellPtr& cell);

ObjPtr BuiltinEq(const CellPtr& cell);

ObjPtr BuiltinAdd(const CellPtr& cell);

ObjPtr BuiltinSub(const CellPtr& cell);

ObjPtr BuiltinMultiply(const CellPtr& cell);

ObjPtr BuiltinDivide(const CellPtr& cell);

ObjPtr BuiltinMax(const CellPtr& cell);

ObjPtr BuiltinMin(const CellPtr& cell);

ObjPtr BuiltinAbs(const CellPtr& cell);