#pragma once

#include "scheme/builtins.h"
#include <string>

template <typename T, typename F>
std::pair<T, F> Result(ObjPtr cell,
                       std::function<std::pair<T, F>(T, ObjPtr)> func,
                       std::function<F(F, F)> merge, T past, F res) {
    if (!cell) {
        return {past, res};
    }
    if (Is<Cell>(cell)) {
        auto in_cell = As<Cell>(cell);
        auto [past_new, res_new] = func(past, in_cell->GetFirst());
        return Result(in_cell->GetSecond(), func, merge, past_new, merge(res, res_new));
    } else {
        auto [past_new, res_new] = func(past, cell);
        return {past_new, merge(res, res_new)};
    }
}

ObjPtr FuncCell(const CellPtr& call);

class Interpreter {
public:
    std::string Run(const std::string &input);
};
