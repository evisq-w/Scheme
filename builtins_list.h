#include "builtins.h"

Obj BuiltinQuote(const CellPtr& cell);

Obj BuiltinIsPair(const CellPtr& cell);

Obj BuiltinIsNull(const CellPtr& cell);

Obj BuiltinIsList(const CellPtr& cell);

Obj BuiltinCons(const CellPtr& cell);

Obj BuiltinCar(const CellPtr& cell);

Obj BuiltinCdr(const CellPtr& cell);

Obj BuiltinList(const CellPtr& cell);

Obj BuiltinListRef(const CellPtr& cell);

Obj BuiltinListTail(const CellPtr& cell);
