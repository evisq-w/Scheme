#include "builtins_integer.h"
#include "error.h"
#include "parser.h"
#include "scheme.h"

Obj BuiltinIsNumber(const CellPtr& cell) {
    auto nextlist = As<Cell>(cell->GetSecond());
    if (!nextlist) {
        throw RuntimeError("Error: Expected at least 1 argument");
    }
    return (Is<Number>(nextlist->GetFirst()) ? MakeS("#t") : MakeS("#f"));
}

Obj BuiltinLess(const CellPtr& cell) {
    return Result<std::shared_ptr<Number>, bool>(
                As<Cell>(cell->GetSecond()),
                [](std::shared_ptr<Number> a,
                    Obj b) -> std::pair<std::shared_ptr<Number>, bool> {
                    if (!Is<Number>(b)) {
                        throw RuntimeError("Error: Expected number");
                    }
                    return {As<Number>(b),
                            As<Number>(a)->GetValue() < As<Number>(b)->GetValue()};
                },
                [](bool a, bool b) -> bool { return a & b; }, MakeN(INT_MIN), true)
                    .second
                ? MakeS("#t")
                : MakeS("#f");

}

Obj BuiltinLessEq(const CellPtr& cell) {
    return Result<std::shared_ptr<Number>, bool>(
                As<Cell>(cell->GetSecond()),
                [](std::shared_ptr<Number> a,
                    Obj b) -> std::pair<std::shared_ptr<Number>, bool> {
                    if (!Is<Number>(b)) {
                        throw RuntimeError("Error: Expected number");
                    }
                    return {As<Number>(b),
                            As<Number>(a)->GetValue() <= As<Number>(b)->GetValue()};
                },
                [](bool a, bool b) -> bool { return a & b; }, MakeN(INT_MIN), true)
                    .second
                ? MakeS("#t")
                : MakeS("#f");
}

Obj BuiltinGreater(const CellPtr& cell) {
    return Result<std::shared_ptr<Number>, bool>(
                As<Cell>(cell->GetSecond()),
                [](std::shared_ptr<Number> a,
                    Obj b) -> std::pair<std::shared_ptr<Number>, bool> {
                    if (!Is<Number>(b)) {
                        throw RuntimeError("Error: Expected number");
                    }
                    return {As<Number>(b),
                            As<Number>(a)->GetValue() > As<Number>(b)->GetValue()};
                },
                [](bool a, bool b) -> bool { return a & b; }, MakeN(INT_MAX), true)
                    .second
                ? MakeS("#t")
                : MakeS("#f");
}

Obj BuiltinGreaterEq(const CellPtr& cell) {
    return Result<std::shared_ptr<Number>, bool>(
                As<Cell>(cell->GetSecond()),
                [](std::shared_ptr<Number> a,
                    Obj b) -> std::pair<std::shared_ptr<Number>, bool> {
                    if (!Is<Number>(b)) {
                        throw RuntimeError("Error: Expected number");
                    }
                    return {As<Number>(b),
                            As<Number>(a)->GetValue() >= As<Number>(b)->GetValue()};
                },
                [](bool a, bool b) -> bool { return a & b; }, MakeN(INT_MAX), true)
                    .second
                ? MakeS("#t")
                : MakeS("#f");
}

Obj BuiltinEq(const CellPtr& cell) {
    if (!cell->GetSecond()) {
        return MakeS("#t");
    }
    auto first_elem = As<Cell>(cell->GetSecond())->GetFirst();
    if (!Is<Number>(first_elem)) {
        throw SyntaxError("Error: Expected number");
    }
    return Result<std::shared_ptr<Number>, bool>(
                As<Cell>(cell->GetSecond()),
                [](std::shared_ptr<Number> a,
                    Obj b) -> std::pair<std::shared_ptr<Number>, bool> {
                    if (!Is<Number>(b)) {
                        throw RuntimeError("Error: Expected number");
                    }
                    return {As<Number>(b),
                            As<Number>(a)->GetValue() == As<Number>(b)->GetValue()};
                },
                [](bool a, bool b) -> bool { return a & b; }, As<Number>(first_elem), true)
                    .second
                ? MakeS("#t")
                : MakeS("#f");
}

Obj BuiltinAdd(const CellPtr& cell) {
    return Result<std::shared_ptr<Number>, bool>(
                As<Cell>(cell->GetSecond()),
                [](std::shared_ptr<Number> a,
                    Obj b) -> std::pair<std::shared_ptr<Number>, bool> {
                    if (Is<Cell>(b)) {
                        b = FuncCell(As<Cell>(b));
                    }
                    if (!Is<Number>(b)) {
                        throw RuntimeError("Error: Expected number");
                    }
                    return {MakeN(a->GetValue() + As<Number>(b)->GetValue()), true};
                },
                [](bool, bool) -> bool { return true; }, MakeN(0), true)
        .first;
}

Obj BuiltinSub(const CellPtr& cell) {
    if (!cell->GetSecond()) {
        throw RuntimeError("Error: Expected number");
    }
    if (!Is<Cell>(cell->GetSecond())) {
        throw SyntaxError("Error: ???");
    }
    auto first_elem = As<Cell>(cell->GetSecond())->GetFirst();
    if (!Is<Number>(first_elem)) {
        throw SyntaxError("Error: Expected number");
    }
    return Result<std::shared_ptr<Number>, bool>(
                As<Cell>(cell->GetSecond()),
                [](std::shared_ptr<Number> a,
                    Obj b) -> std::pair<std::shared_ptr<Number>, bool> {
                    if (Is<Cell>(b)) {
                        b = FuncCell(As<Cell>(b));
                    }
                    if (!Is<Number>(b)) {
                        throw RuntimeError("Error: Expected number");
                    }
                    return {MakeN(a->GetValue() - As<Number>(b)->GetValue()), true};
                },
                [](bool, bool) -> bool { return true; },
                MakeN(2 * As<Number>(first_elem)->GetValue()), true)
        .first;
}

Obj BuiltinMultiply(const CellPtr& cell) {
    return Result<std::shared_ptr<Number>, bool>(
                As<Cell>(cell->GetSecond()),
                [](std::shared_ptr<Number> a,
                    Obj b) -> std::pair<std::shared_ptr<Number>, bool> {
                    if (Is<Cell>(b)) {
                        b = FuncCell(As<Cell>(b));
                    }
                    if (!Is<Number>(b)) {
                        throw RuntimeError("Error: Expected number");
                    }
                    return {MakeN(a->GetValue() * As<Number>(b)->GetValue()), true};
                },
                [](bool, bool) -> bool { return true; }, MakeN(1), true)
        .first;
}

Obj BuiltinDivide(const CellPtr& cell) {
    if (!cell->GetSecond()) {
        throw RuntimeError("Error: Expected number");
    }
    if (!Is<Cell>(cell->GetSecond())) {
        throw SyntaxError("Error: ???");
    }
    auto first_elem = As<Cell>(cell->GetSecond())->GetFirst();
    if (!Is<Number>(first_elem)) {
        throw SyntaxError("Error: Expected number");
    }
    int num = (As<Number>(first_elem)->GetValue());
    return Result<std::shared_ptr<Number>, bool>(
                As<Cell>(cell->GetSecond()),
                [](std::shared_ptr<Number> a,
                    Obj b) -> std::pair<std::shared_ptr<Number>, bool> {
                    if (Is<Cell>(b)) {
                        b = FuncCell(As<Cell>(b));
                    }
                    if (!Is<Number>(b)) {
                        throw RuntimeError("Error: Expected number");
                    }
                    return {MakeN(a->GetValue() / As<Number>(b)->GetValue()), true};
                },
                [](bool, bool) -> bool { return true; }, MakeN(num * num), true)
        .first;
}

Obj BuiltinMax(const CellPtr& cell) {
    if (!cell->GetSecond()) {
        throw RuntimeError("Error: Expected number");
    }
    return Result<std::shared_ptr<Number>, bool>(
                As<Cell>(cell->GetSecond()),
                [](std::shared_ptr<Number> a,
                    Obj b) -> std::pair<std::shared_ptr<Number>, bool> {
                    if (!Is<Number>(b)) {
                        throw RuntimeError("Error: Expected number");
                    }
                    return {MakeN(std::max<int>(a->GetValue(), As<Number>(b)->GetValue())),
                            true};
                },
                [](bool a, bool b) -> bool { return a & b; }, MakeN(INT_MIN), true)
        .first;
}

Obj BuiltinMin(const CellPtr& cell) {
    if (!cell->GetSecond()) {
        throw RuntimeError("Error: Expected number");
    }
    return Result<std::shared_ptr<Number>, bool>(
                As<Cell>(cell->GetSecond()),
                [](std::shared_ptr<Number> a,
                    Obj b) -> std::pair<std::shared_ptr<Number>, bool> {
                    if (!Is<Number>(b)) {
                        throw RuntimeError("Error: Expected number");
                    }
                    return {MakeN(std::min<int>(a->GetValue(), As<Number>(b)->GetValue())),
                            true};
                },
                [](bool a, bool b) -> bool { return a & b; }, MakeN(INT_MAX), true)
        .first;

}

Obj BuiltinAbs(const CellPtr& cell) {
    auto nextlist = As<Cell>(cell->GetSecond());
    if (!nextlist || nextlist->GetSecond()) {
        throw RuntimeError("Error: Expected 1 argument");
    }
    if (!nextlist->GetFirst() || !Is<Number>(nextlist->GetFirst())) {
        throw RuntimeError("Error: Expected int argument");
    }
    return MakeN(std::abs(As<Number>(nextlist->GetFirst())->GetValue()));
}