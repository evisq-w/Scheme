#include "boolean.h"
#include "object.h"

bool IsTrue(ObjPtr val) {
    if (Is<Number>(val)) {
        return (As<Number>(val)->GetValue() != 0);
    }
    if (Is<Symbol>(val)) {
        return (As<Symbol>(val)->GetName() != "#f");
    }
    return true;
}

bool IsBool(ObjPtr val) {
    if (Is<Symbol>(val)) {
        std::string value = As<Symbol>(val)->GetName();
        return (value == "#f" || value == "#t");
    }
    return false;
}


