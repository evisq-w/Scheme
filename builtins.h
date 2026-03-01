#pragma once

#include "object.h"
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

using ObjPtr = std::shared_ptr<Object>;
using CellPtr = std::shared_ptr<Cell>;
using Builtin = std::function<ObjPtr(const CellPtr& call)>;

const std::unordered_map<std::string, Builtin>& GetBuiltins();