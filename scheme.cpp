#include "boolean.h"
#include "error.h"
#include "object.h"
#include "parser.h"
#include "printer.h"
#include "scheme.h"
#include <tokenizer.h>
#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>

Obj FuncCall(const CellPtr& call) {
    if (!call || !Is<Symbol>(call->GetFirst())) {
        throw RuntimeError("Expected function name");
    }
    const std::string& name = As<Symbol>(call->GetFirst())->GetName();

    const auto& builtins = GetBuiltins();
    auto it = builtins.find(name);
    if (it == builtins.end()) {
        throw RuntimeError("Unknown function: " + name);
    }
    return it->second(call);
}

std::string Interpreter::Run(const std::string &input) {
    std::stringstream sstr{input};
    Tokenizer tokenizer(&sstr);
    Obj res = Read(&tokenizer);
    if (Is<Number>(res)) {
        return Itos(As<Number>(res)->GetValue());
    }
    if (Is<Symbol>(res)) {
        return As<Symbol>(res)->GetName();
    }
    if (Is<Cell>(res)) {
        auto obj = FuncCall(As<Cell>(res));
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
