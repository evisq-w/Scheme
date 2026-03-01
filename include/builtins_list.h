#pragma once

#include "builtins.h"

ObjPtr BuiltinQuote(const CellPtr& cell);

ObjPtr BuiltinIsPair(const CellPtr& cell);

ObjPtr BuiltinIsNull(const CellPtr& cell);

ObjPtr BuiltinIsList(const CellPtr& cell);

ObjPtr BuiltinCons(const CellPtr& cell);

ObjPtr BuiltinCar(const CellPtr& cell);

ObjPtr BuiltinCdr(const CellPtr& cell);

ObjPtr BuiltinList(const CellPtr& cell);

ObjPtr BuiltinListRef(const CellPtr& cell);

ObjPtr BuiltinListTail(const CellPtr& cell);
