#include "printer.h"

std::string Itos(int32_t var) {
    if (var == 0) {
        return "0";
    }
    bool neg = false;
    std::string res{};
    if (var < 0) {
        var *= -1;
        neg = true;
    }
    while (var > 0) {
        res.push_back('0' + (var % 10));
        var /= 10;
    }
    if (neg) {
        res.push_back('-');
    }
    reverse(res.begin(), res.end());
    return res;
}


std::string PrintObj(ObjPtr res) {
    if (!res) {
        return "()";
    }
    if (Is<Number>(res)) {
        return Itos(As<Number>(res)->GetValue());
    }
    if (Is<Symbol>(res)) {
        return As<Symbol>(res)->GetName();
    }
    if (Is<Cell>(res)) {
        auto res_cell = As<Cell>(res);
        auto first = res_cell->GetFirst();
        if (Is<Symbol>(first) && As<Symbol>(first)->GetName() == "quote") {
            return "'" + PrintObj(res_cell->GetSecond());
        }
        return "(" + PrintList(res_cell) + ")";
    }
    return "";
}

std::string PrintList(CellPtr cell) {
    std::string first = (!cell->GetFirst() ? "()" : PrintObj(cell->GetFirst()));
    if (!cell->GetSecond()) {
        return first;
    }
    if (Is<Cell>(cell->GetSecond())) {
        return first + " " + PrintList(As<Cell>(cell->GetSecond()));
    }
    return first + " . " + PrintObj(cell->GetSecond());
}