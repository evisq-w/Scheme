#include "builtins.h"
#include "builtins_boolean.h"
#include "builtins_integer.h"
#include "builtins_list.h"

static const std::unordered_map<std::string, Builtin> kBuiltins = {
    {"quote", BuiltinQuote},
    {"boolean?", BuiltinIsBoolean},
    {"not", BuiltinNotBool},
    {"and", BuiltinAndBool},
    {"or", BuiltinOrBool},
    {"number?", BuiltinIsNumber},
    {"<", BuiltinLess},
    {"<=", BuiltinLessEq},
    {">", BuiltinGreater},
    {">=", BuiltinGreaterEq},
    {"=", BuiltinEq},
    {"+", BuiltinAdd},
    {"-", BuiltinSub},
    {"*", BuiltinMultiply},
    {"/", BuiltinDivide},
    {"max", BuiltinMax},
    {"min", BuiltinMin},
    {"abs", BuiltinAbs},
    {"pair?", BuiltinIsPair},
    {"null?", BuiltinIsNull},
    {"list?", BuiltinIsList},
    {"cons", BuiltinCons},
    {"car", BuiltinCar},
    {"cdr", BuiltinCdr},
    {"list", BuiltinList},
    {"list-ref", BuiltinListRef},
    {"list-tail", BuiltinListTail}
};

const std::unordered_map<std::string, Builtin>& GetBuiltins() {
    return kBuiltins;
}