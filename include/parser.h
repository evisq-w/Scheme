#pragma once

#include <memory>

#include "scheme/object.h"
#include "scheme/tokenizer.h"

std::shared_ptr<Symbol> MakeS(std::string input);
std::shared_ptr<Number> MakeN(int32_t input);
std::shared_ptr<Cell> MakeC(std::shared_ptr<Object> a, std::shared_ptr<Object> b);

std::shared_ptr<Object> Read(Tokenizer* tokenizer);
std::shared_ptr<Object> ReadList(Tokenizer* tokenizer);