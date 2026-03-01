#include "builtins_list.h"
#include "error.h"
#include "parser.h"
#include "scheme.h"

ObjPtr BuiltinQuote(const CellPtr& cell) {
    return cell->GetSecond();
}

ObjPtr BuiltinIsPair(const CellPtr& cell) {
    auto nextlist = As<Cell>(cell->GetSecond());
    if (!nextlist || nextlist->GetSecond()) {
        throw RuntimeError("Error: Expected 1 argument");
    }
    if (!nextlist->GetFirst() || !Is<Cell>(nextlist->GetFirst())) {
        return MakeS("#f");
    }
    auto itog = FuncCell(As<Cell>(nextlist->GetFirst()));
    if (Is<Cell>(itog)) {
        auto itog_cell = As<Cell>(itog);
        if (!Is<Cell>(itog_cell->GetSecond()) ||
            !As<Cell>(itog_cell->GetSecond())->GetSecond()) {
            return MakeS("#t");
        }
    }
    return MakeS("#f");
}

ObjPtr BuiltinIsNull(const CellPtr& cell) {
    auto nextlist = As<Cell>(cell->GetSecond());
    if (!nextlist || nextlist->GetSecond()) {
        throw RuntimeError("Error: Expected 1 argument");
    }
    if (!nextlist->GetFirst() || !Is<Cell>(nextlist->GetFirst())) {
        return MakeS("#f");
    }
    auto itog = FuncCell(As<Cell>(nextlist->GetFirst()));
    return (itog == nullptr ? MakeS("#t") : MakeS("#f"));   
}

ObjPtr BuiltinIsList(const CellPtr& cell) {
    auto nextlist = As<Cell>(cell->GetSecond());
    if (!nextlist || nextlist->GetSecond()) {
        throw RuntimeError("Error: Expected 1 argument");
    }
    if (!nextlist->GetFirst() || !Is<Cell>(nextlist->GetFirst())) {
        return MakeS("#f");
    }
    auto itog = FuncCell(As<Cell>(nextlist->GetFirst()));
    while (Is<Cell>(itog)) {
        itog = As<Cell>(itog)->GetSecond();
    }
    return (itog == nullptr ? MakeS("#t") : MakeS("#f"));
}

ObjPtr BuiltinCons(const CellPtr& cell) {
    auto nextlist = As<Cell>(cell->GetSecond());
    if (!nextlist || !nextlist->GetSecond()) {
        throw RuntimeError("Error: Expected 2 argument");
    }
    if (!Is<Cell>(nextlist->GetSecond())) {
        return nextlist;
    } else {
        return MakeC(nextlist->GetFirst(), As<Cell>(nextlist->GetSecond())->GetFirst());
    }
}

ObjPtr BuiltinCar(const CellPtr& cell) {
    auto nextlist = As<Cell>(cell->GetSecond());
    if (!nextlist) {
        throw RuntimeError("Error: Expected at least 1 argument");
    }
    if (!nextlist->GetFirst() || !Is<Cell>(nextlist->GetFirst())) {
        throw RuntimeError("Expected list");
    }
    auto itog = FuncCell(As<Cell>(nextlist->GetFirst()));
    if (Is<Cell>(itog)) {
        if (As<Cell>(itog)->GetFirst()) {
            return As<Cell>(itog)->GetFirst();
        }
        throw RuntimeError("Error: Expected at least 1 variable");
    }
    if (!itog) {
        throw RuntimeError("Error: Expected at least 1 variable");
    }
    return itog;
}

ObjPtr BuiltinCdr(const CellPtr& cell) {
    auto nextlist = As<Cell>(cell->GetSecond());
    if (!nextlist) {
        throw RuntimeError("Error: Expected at least 1 argument");
    }
    if (!nextlist->GetFirst() || !Is<Cell>(nextlist->GetFirst())) {
        throw RuntimeError("Expected list");
    }
    auto itog = FuncCell(As<Cell>(nextlist->GetFirst()));
    if (Is<Cell>(itog)) {
        if (As<Cell>(itog)->GetFirst()) {
            return As<Cell>(itog)->GetSecond();
        }
        throw RuntimeError("Error: Expected at least 1 variable");
    }
    if (!itog) {
        throw RuntimeError("Error: Expected at least 1 variable");
    }
    return nullptr;
}

ObjPtr BuiltinList(const CellPtr& cell) {
    return As<Cell>(cell->GetSecond());
}

ObjPtr BuiltinListRef(const CellPtr& cell) {
    auto nextlist = As<Cell>(cell->GetSecond());
    if (!nextlist) {
        throw RuntimeError("Error: Expected at least 2 argument");
    }
    if (!nextlist->GetFirst() || !Is<Cell>(nextlist->GetFirst())) {
        throw RuntimeError("Expected list as first arg");
    }
    if (!nextlist->GetSecond() || !Is<Cell>(nextlist->GetSecond()) ||
        !Is<Number>(As<Cell>(nextlist->GetSecond())->GetFirst())) {
        throw RuntimeError("Expected number as second arg");
    }
    int64_t cnt = As<Number>(As<Cell>(nextlist->GetSecond())->GetFirst())->GetValue();
    auto itog = FuncCell(As<Cell>(nextlist->GetFirst()));
    while (cnt--) {
        if (!Is<Cell>(itog)) {
            throw RuntimeError("Not enough elements");
        }
        itog = As<Cell>(itog)->GetSecond();
    }
    if (!itog) {
        throw RuntimeError("Not enough elements");
    }
    if (!Is<Cell>(itog)) {
        return itog;
    }
    return As<Cell>(itog)->GetFirst();
}

ObjPtr BuiltinListTail(const CellPtr& cell) {
    auto nextlist = As<Cell>(cell->GetSecond());
    if (!nextlist) {
        throw RuntimeError("Error: Expected at least 2 argument");
    }
    if (!nextlist->GetFirst() || !Is<Cell>(nextlist->GetFirst())) {
        throw RuntimeError("Expected list as first arg");
    }
    if (!nextlist->GetSecond() || !Is<Cell>(nextlist->GetSecond()) ||
        !Is<Number>(As<Cell>(nextlist->GetSecond())->GetFirst())) {
        throw RuntimeError("Expected number as second arg");
    }
    int64_t cnt = As<Number>(As<Cell>(nextlist->GetSecond())->GetFirst())->GetValue();
    auto itog = FuncCell(As<Cell>(nextlist->GetFirst()));
    while (cnt--) {
        if (!Is<Cell>(itog)) {
            throw RuntimeError("Not enough elements");
        }
        itog = As<Cell>(itog)->GetSecond();
    }
    return itog;
}
