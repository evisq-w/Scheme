#pragma once

#include "builtins.h"

ObjPtr BuiltinIsBoolean(const CellPtr& cell);

ObjPtr BuiltinNotBool(const CellPtr& cell);

ObjPtr BuiltinAndBool(const CellPtr& cell);

ObjPtr BuiltinOrBool(const CellPtr& cell);