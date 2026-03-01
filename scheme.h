#pragma once

#include <stdexcept>
#include <string>
#include <variant>

class Interpreter {
public:
    std::string Run(const std::string &input);
};
