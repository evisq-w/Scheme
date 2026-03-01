#include "boolean.h"
#include "builtins_boolean.h"
#include "error.h"
#include "parser.h"
#include "scheme.h"

ObjPtr BuiltinIsBoolean(const CellPtr& cell) {
    auto nextlist = As<Cell>(cell->GetSecond());
    if (!nextlist) {
        throw RuntimeError("Error: Expected at least 1 argument");
    }
    if (nextlist->GetSecond()) {
        throw RuntimeError("Error: Expected 1 argument");
    }
    if (Is<Symbol>(nextlist->GetFirst())) {
        std::string name = (As<Symbol>(nextlist->GetFirst()))->GetName();
        return (name == "#t" || name == "#f" ? MakeS("#t") : MakeS("#f"));
    }
    return MakeS("#f");
}

ObjPtr BuiltinNotBool(const CellPtr& cell) {
    auto nextlist = As<Cell>(cell->GetSecond());
    if (!nextlist) {
        throw RuntimeError("Error: Expected at least 1 argument");
    }
    if (nextlist->GetSecond()) {
        throw RuntimeError("Error: Expected 1 argument");
    }
    if (!nextlist->GetFirst()) {
        return MakeS("#t");
    }
    if (Is<Symbol>(nextlist->GetFirst())) {
        std::string name = (As<Symbol>(nextlist->GetFirst()))->GetName();
        return (name == "#f" ? MakeS("#t") : MakeS("#f"));
    }
    return MakeS("#f");
}

ObjPtr BuiltinAndBool(const CellPtr& cell) {
    return Result<ObjPtr, bool>(
                As<Cell>(cell->GetSecond()),
                [](ObjPtr a,
                    ObjPtr b) -> std::pair<ObjPtr, bool> {
                    if (!IsTrue(a)) {
                        return {a, false};
                    }
                    if (Is<Cell>(b)) {
                        b = FuncCell(As<Cell>(b));
                    }
                    return {b, IsTrue(b)};
                },
                [](bool a, bool b) -> bool { return a & b; }, MakeS("#t"), true)
        .first;
}


ObjPtr BuiltinOrBool(const CellPtr& cell) {
    return Result<ObjPtr, bool>(
                As<Cell>(cell->GetSecond()),
                [](ObjPtr a,
                    ObjPtr b) -> std::pair<ObjPtr, bool> {
                    if (IsTrue(a)) {
                        return {a, true};
                    }
                    if (Is<Cell>(b)) {
                        b = FuncCell(As<Cell>(b));
                    }
                    return {b, IsTrue(b)};
                },
                [](bool a, bool b) -> bool { return a || b; }, MakeS("#f"), true)
        .first;
}