#include <parser.h>
#include <memory>
#include <stdexcept>
#include "error.h"
#include "object.h"
#include "tokenizer.h"

std::shared_ptr<Symbol> MakeS(std::string input) {
    return std::make_shared<Symbol>(input);
}

std::shared_ptr<Number> MakeN(int32_t input) {
    return std::make_shared<Number>(input);
}

std::shared_ptr<Cell> MakeC(std::shared_ptr<Object> a, std::shared_ptr<Object> b) {
    return std::make_shared<Cell>(a, b);
}

std::shared_ptr<Object> Read(Tokenizer* tokenizer) {
    if (tokenizer->IsEnd()) {
        throw SyntaxError("Empty input");
    }
    Token tok = tokenizer->GetToken();
    tokenizer->Next();

    if (ConstantToken* x = std::get_if<ConstantToken>(&tok)) {
        return std::make_shared<Number>(x->value);
    }

    if (BracketToken* x = std::get_if<BracketToken>(&tok)) {
        if (*x == BracketToken::OPEN) {
            return ReadList(tokenizer);
        } else {
            throw SyntaxError("Expected opened bracket or next element");
        }
    }

    if (SymbolToken* x = std::get_if<SymbolToken>(&tok)) {
        return std::make_shared<Symbol>(x->name);
    }
    if (DotToken* _ = std::get_if<DotToken>(&tok)) {
        throw SyntaxError("Expected closed bracket or next element");
    }

    if (QuoteToken* _ = std::get_if<QuoteToken>(&tok)) {
        return std::make_shared<Cell>(std::make_shared<Symbol>("quote"), Read(tokenizer));
    }

    throw SyntaxError("SyntaxError");
}

std::shared_ptr<Object> ReadList(Tokenizer* tokenizer) {
    if (tokenizer->IsEnd()) {
        throw SyntaxError("Expected closed bracket");
    }
    Token tok = tokenizer->GetToken();
    if (BracketToken* x = std::get_if<BracketToken>(&tok)) {
        if (*x == BracketToken::CLOSE) {
            tokenizer->Next();
            return nullptr;
        }
    }
    auto first = Read(tokenizer);
    if (tokenizer->IsEnd()) {
        throw SyntaxError("Expected closed bracket or next element");
    }
    tok = tokenizer->GetToken();
    if (BracketToken* x = std::get_if<BracketToken>(&tok)) {
        if (*x == BracketToken::CLOSE) {
            tokenizer->Next();
            return std::make_shared<Cell>(first, nullptr);
        }
    } else if (DotToken* _ = std::get_if<DotToken>(&tok)) {
        tokenizer->Next();
        if (tokenizer->IsEnd()) {
            throw SyntaxError("Expected closed bracket");
        }
        auto second = Read(tokenizer);
        if (tokenizer->IsEnd()) {
            throw SyntaxError("Expected closed bracket");
        }
        tok = tokenizer->GetToken();
        if (BracketToken* x = std::get_if<BracketToken>(&tok)) {
            if (*x == BracketToken::CLOSE) {
                tokenizer->Next();
                return std::make_shared<Cell>(first, second);
            }
        }
        throw SyntaxError("Expected closed bracket");
    }
    if (Is<Symbol>(first) && As<Symbol>(first)->GetName() == "quote") {
        auto second = Read(tokenizer);
        return MakeC(first, second);
    }
    auto second = ReadList(tokenizer);
    return std::make_shared<Cell>(first, second);
}