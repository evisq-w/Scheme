#include "scheme.h"
#include "error.h"
#include "object.h"
#include "parser.h"
#include "tokenizer.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>

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

template <typename T, typename F>
std::pair<T, F> Result(std::shared_ptr<Object> cell,
                       std::function<std::pair<T, F>(T, std::shared_ptr<Object>)> func,
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

bool CheckTrue(std::shared_ptr<Object> val) {
    if (Is<Number>(val)) {
        return (As<Number>(val)->GetValue() != 0);
    }
    if (Is<Symbol>(val)) {
        return (As<Symbol>(val)->GetName() != "#f");
    }
    return true;
}

std::shared_ptr<Object> RunCell(std::shared_ptr<Cell> cell) {
    if (Is<Symbol>(cell->GetFirst())) {
        auto command = As<Symbol>(cell->GetFirst());
        if (command->GetName() == "quote") {
            return cell->GetSecond();
        }

        if (command->GetName() == "boolean?") {
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

        if (command->GetName() == "not") {
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

        if (command->GetName() == "and") {
            return Result<std::shared_ptr<Object>, bool>(
                       As<Cell>(cell->GetSecond()),
                       [](std::shared_ptr<Object> a,
                          std::shared_ptr<Object> b) -> std::pair<std::shared_ptr<Object>, bool> {
                           if (!CheckTrue(a)) {
                               return {a, false};
                           }
                           if (Is<Cell>(b)) {
                               b = RunCell(As<Cell>(b));
                           }
                           return {b, CheckTrue(b)};
                       },
                       [](bool a, bool b) -> bool { return a & b; }, MakeS("#t"), true)
                .first;
        }

        if (command->GetName() == "or") {
            return Result<std::shared_ptr<Object>, bool>(
                       As<Cell>(cell->GetSecond()),
                       [](std::shared_ptr<Object> a,
                          std::shared_ptr<Object> b) -> std::pair<std::shared_ptr<Object>, bool> {
                           if (CheckTrue(a)) {
                               return {a, true};
                           }
                           if (Is<Cell>(b)) {
                               b = RunCell(As<Cell>(b));
                           }
                           return {b, CheckTrue(b)};
                       },
                       [](bool a, bool b) -> bool { return a || b; }, MakeS("#f"), true)
                .first;
        }

        if (command->GetName() == "number?") {
            auto nextlist = As<Cell>(cell->GetSecond());
            if (!nextlist) {
                throw RuntimeError("Error: Expected at least 1 argument");
            }
            return (Is<Number>(nextlist->GetFirst()) ? MakeS("#t") : MakeS("#f"));
        }

        if (command->GetName() == "<") {
            return Result<std::shared_ptr<Number>, bool>(
                       As<Cell>(cell->GetSecond()),
                       [](std::shared_ptr<Number> a,
                          std::shared_ptr<Object> b) -> std::pair<std::shared_ptr<Number>, bool> {
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
        if (command->GetName() == "<=") {
            return Result<std::shared_ptr<Number>, bool>(
                       As<Cell>(cell->GetSecond()),
                       [](std::shared_ptr<Number> a,
                          std::shared_ptr<Object> b) -> std::pair<std::shared_ptr<Number>, bool> {
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
        if (command->GetName() == ">") {
            return Result<std::shared_ptr<Number>, bool>(
                       As<Cell>(cell->GetSecond()),
                       [](std::shared_ptr<Number> a,
                          std::shared_ptr<Object> b) -> std::pair<std::shared_ptr<Number>, bool> {
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
        if (command->GetName() == ">=") {
            return Result<std::shared_ptr<Number>, bool>(
                       As<Cell>(cell->GetSecond()),
                       [](std::shared_ptr<Number> a,
                          std::shared_ptr<Object> b) -> std::pair<std::shared_ptr<Number>, bool> {
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
        if (command->GetName() == "=") {
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
                          std::shared_ptr<Object> b) -> std::pair<std::shared_ptr<Number>, bool> {
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

        if (command->GetName() == "+") {
            return Result<std::shared_ptr<Number>, bool>(
                       As<Cell>(cell->GetSecond()),
                       [](std::shared_ptr<Number> a,
                          std::shared_ptr<Object> b) -> std::pair<std::shared_ptr<Number>, bool> {
                           if (Is<Cell>(b)) {
                               b = RunCell(As<Cell>(b));
                           }
                           if (!Is<Number>(b)) {
                               throw RuntimeError("Error: Expected number");
                           }
                           return {MakeN(a->GetValue() + As<Number>(b)->GetValue()), true};
                       },
                       [](bool, bool) -> bool { return true; }, MakeN(0), true)
                .first;
        }
        if (command->GetName() == "-") {
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
                          std::shared_ptr<Object> b) -> std::pair<std::shared_ptr<Number>, bool> {
                           if (Is<Cell>(b)) {
                               b = RunCell(As<Cell>(b));
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
        if (command->GetName() == "*") {
            return Result<std::shared_ptr<Number>, bool>(
                       As<Cell>(cell->GetSecond()),
                       [](std::shared_ptr<Number> a,
                          std::shared_ptr<Object> b) -> std::pair<std::shared_ptr<Number>, bool> {
                           if (Is<Cell>(b)) {
                               b = RunCell(As<Cell>(b));
                           }
                           if (!Is<Number>(b)) {
                               throw RuntimeError("Error: Expected number");
                           }
                           return {MakeN(a->GetValue() * As<Number>(b)->GetValue()), true};
                       },
                       [](bool, bool) -> bool { return true; }, MakeN(1), true)
                .first;
        }
        if (command->GetName() == "/") {
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
                          std::shared_ptr<Object> b) -> std::pair<std::shared_ptr<Number>, bool> {
                           if (Is<Cell>(b)) {
                               b = RunCell(As<Cell>(b));
                           }
                           if (!Is<Number>(b)) {
                               throw RuntimeError("Error: Expected number");
                           }
                           return {MakeN(a->GetValue() / As<Number>(b)->GetValue()), true};
                       },
                       [](bool, bool) -> bool { return true; }, MakeN(num * num), true)
                .first;
        }

        if (command->GetName() == "max") {
            if (!cell->GetSecond()) {
                throw RuntimeError("Error: Expected number");
            }
            return Result<std::shared_ptr<Number>, bool>(
                       As<Cell>(cell->GetSecond()),
                       [](std::shared_ptr<Number> a,
                          std::shared_ptr<Object> b) -> std::pair<std::shared_ptr<Number>, bool> {
                           if (!Is<Number>(b)) {
                               throw RuntimeError("Error: Expected number");
                           }
                           return {MakeN(std::max<int>(a->GetValue(), As<Number>(b)->GetValue())),
                                   true};
                       },
                       [](bool a, bool b) -> bool { return a & b; }, MakeN(INT_MIN), true)
                .first;
        }
        if (command->GetName() == "min") {
            if (!cell->GetSecond()) {
                throw RuntimeError("Error: Expected number");
            }
            return Result<std::shared_ptr<Number>, bool>(
                       As<Cell>(cell->GetSecond()),
                       [](std::shared_ptr<Number> a,
                          std::shared_ptr<Object> b) -> std::pair<std::shared_ptr<Number>, bool> {
                           if (!Is<Number>(b)) {
                               throw RuntimeError("Error: Expected number");
                           }
                           return {MakeN(std::min<int>(a->GetValue(), As<Number>(b)->GetValue())),
                                   true};
                       },
                       [](bool a, bool b) -> bool { return a & b; }, MakeN(INT_MAX), true)
                .first;
        }

        if (command->GetName() == "abs") {
            auto nextlist = As<Cell>(cell->GetSecond());
            if (!nextlist || nextlist->GetSecond()) {
                throw RuntimeError("Error: Expected 1 argument");
            }
            if (!nextlist->GetFirst() || !Is<Number>(nextlist->GetFirst())) {
                throw RuntimeError("Error: Expected int argument");
            }
            return MakeN(std::abs(As<Number>(nextlist->GetFirst())->GetValue()));
        }

        if (command->GetName() == "pair?") {
            auto nextlist = As<Cell>(cell->GetSecond());
            if (!nextlist || nextlist->GetSecond()) {
                throw RuntimeError("Error: Expected 1 argument");
            }
            if (!nextlist->GetFirst() || !Is<Cell>(nextlist->GetFirst())) {
                return MakeS("#f");
            }
            auto itog = RunCell(As<Cell>(nextlist->GetFirst()));
            if (Is<Cell>(itog)) {
                auto itog_cell = As<Cell>(itog);
                if (!Is<Cell>(itog_cell->GetSecond()) ||
                    !As<Cell>(itog_cell->GetSecond())->GetSecond()) {
                    return MakeS("#t");
                }
            }
            return MakeS("#f");
        }

        if (command->GetName() == "null?") {
            auto nextlist = As<Cell>(cell->GetSecond());
            if (!nextlist || nextlist->GetSecond()) {
                throw RuntimeError("Error: Expected 1 argument");
            }
            if (!nextlist->GetFirst() || !Is<Cell>(nextlist->GetFirst())) {
                return MakeS("#f");
            }
            auto itog = RunCell(As<Cell>(nextlist->GetFirst()));
            return (itog == nullptr ? MakeS("#t") : MakeS("#f"));
        }

        if (command->GetName() == "list?") {
            auto nextlist = As<Cell>(cell->GetSecond());
            if (!nextlist || nextlist->GetSecond()) {
                throw RuntimeError("Error: Expected 1 argument");
            }
            if (!nextlist->GetFirst() || !Is<Cell>(nextlist->GetFirst())) {
                return MakeS("#f");
            }
            auto itog = RunCell(As<Cell>(nextlist->GetFirst()));
            while (Is<Cell>(itog)) {
                itog = As<Cell>(itog)->GetSecond();
            }
            return (itog == nullptr ? MakeS("#t") : MakeS("#f"));
        }

        if (command->GetName() == "cons") {
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

        if (command->GetName() == "car") {
            auto nextlist = As<Cell>(cell->GetSecond());
            if (!nextlist) {
                throw RuntimeError("Error: Expected at least 1 argument");
            }
            if (!nextlist->GetFirst() || !Is<Cell>(nextlist->GetFirst())) {
                throw RuntimeError("Expected list");
            }
            auto itog = RunCell(As<Cell>(nextlist->GetFirst()));
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

        if (command->GetName() == "cdr") {
            auto nextlist = As<Cell>(cell->GetSecond());
            if (!nextlist) {
                throw RuntimeError("Error: Expected at least 1 argument");
            }
            if (!nextlist->GetFirst() || !Is<Cell>(nextlist->GetFirst())) {
                throw RuntimeError("Expected list");
            }
            auto itog = RunCell(As<Cell>(nextlist->GetFirst()));
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

        if (command->GetName() == "list") {
            auto nextlist = As<Cell>(cell->GetSecond());
            return nextlist;
        }

        if (command->GetName() == "list-ref") {
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
            int cnt = As<Number>(As<Cell>(nextlist->GetSecond())->GetFirst())->GetValue();
            auto itog = RunCell(As<Cell>(nextlist->GetFirst()));
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

        if (command->GetName() == "list-tail") {
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
            int cnt = As<Number>(As<Cell>(nextlist->GetSecond())->GetFirst())->GetValue();
            auto itog = RunCell(As<Cell>(nextlist->GetFirst()));
            while (cnt--) {
                if (!Is<Cell>(itog)) {
                    throw RuntimeError("Not enough elements");
                }
                itog = As<Cell>(itog)->GetSecond();
            }
            return itog;
        }
    }
    throw RuntimeError("Error: Such command doesn't exist");
}

std::string PrintList(std::shared_ptr<Cell> cell);

std::string PrintObj(std::shared_ptr<Object> res) {
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

std::string PrintList(std::shared_ptr<Cell> cell) {
    std::string first = (!cell->GetFirst() ? "()" : PrintObj(cell->GetFirst()));
    if (!cell->GetSecond()) {
        return first;
    }
    if (Is<Cell>(cell->GetSecond())) {
        return first + " " + PrintList(As<Cell>(cell->GetSecond()));
    }
    return first + " . " + PrintObj(cell->GetSecond());
}

std::string Interpreter::Run(const std::string &input) {
    std::stringstream sstr{input};
    Tokenizer tokenizer(&sstr);
    std::shared_ptr<Object> res = Read(&tokenizer);
    if (Is<Number>(res)) {
        return Itos(As<Number>(res)->GetValue());
    }
    if (Is<Symbol>(res)) {
        return As<Symbol>(res)->GetName();
    }
    if (Is<Cell>(res)) {
        auto obj = RunCell(As<Cell>(res));
        if (Is<Symbol>(obj)) {
            return As<Symbol>(obj)->GetName();
        } else if (Is<Number>(obj)) {
            return Itos(As<Number>(obj)->GetValue());
        } else {
            return PrintObj(As<Cell>(obj));
        }
    }
    return "()";
}
