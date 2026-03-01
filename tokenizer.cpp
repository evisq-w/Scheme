#include "tokenizer.h"
#include <cctype>
#include <istream>

bool SymbolToken::operator==(const SymbolToken& other) const {
    return (name == other.name);
}

bool ConstantToken::operator==(const ConstantToken& other) const {
    return (value == other.value);
}

bool QuoteToken::operator==(const QuoteToken&) const {
    return true;
}

bool DotToken::operator==(const DotToken&) const {
    return true;
}

Tokenizer::Tokenizer(std::istream* in) : in_(in) {
    Next();
}

bool Tokenizer::IsEnd() {
    return end_;
}

int64_t GetDigit(std::istream* in) {
    int32_t res = 0;
    auto symb = in->peek();
    while (isdigit(symb)) {
        res = res * 10 + (symb - '0');
        in->get();
        symb = in->peek();
    }
    return res;
}

std::string GetString(std::istream* in) {
    std::string res{};
    char symb = in->peek();
    while (!in->eof() && (('a' <= symb && symb <= 'z') || ('A' <= symb && symb <= 'Z') ||
                          ('0' <= symb && symb <= '9') || symb == '<' || symb == '>' ||
                          symb == '=' || symb == '*' || symb == '/' || symb == '#' || symb == '?' ||
                          symb == '!' || symb == '-' || symb == '[' || symb == ']')) {
        res.push_back(symb);
        in->get();
        symb = in->peek();
    }
    return res;
}

void Tokenizer::Next() {
    while (std::isspace(in_->peek())) {
        in_->get();
    }
    if (in_->eof()) {
        end_ = true;
        return;
    }
    char symb = in_->peek();
    if (symb == '(' || symb == ')') {
        token_ = (symb == '(' ? BracketToken::OPEN : BracketToken::CLOSE);
        in_->get();
        return;
    }
    if (symb == '.') {
        token_ = DotToken{};
        in_->get();
        return;
    }
    if (symb == '\'') {
        token_ = QuoteToken{};
        in_->get();
        return;
    }
    if (symb == '-') {
        in_->get();
        symb = in_->peek();
        if (std::isdigit(symb)) {
            token_ = ConstantToken{-GetDigit(in_)};
        } else if (std::isspace(symb)) {
            token_ = SymbolToken{"-"};
        } else {
            token_ = SymbolToken{"-" + GetString(in_)};
        }
        return;
    }
    if (symb == '+') {
        in_->get();
        symb = in_->peek();
        if (std::isdigit(symb)) {
            token_ = ConstantToken{GetDigit(in_)};
        } else {
            token_ = SymbolToken{"+" + GetString(in_)};
        }
        return;
    }
    if (std::isdigit(symb)) {
        token_ = ConstantToken{GetDigit(in_)};
        return;
    }
    token_ = SymbolToken{GetString(in_)};
}

Token Tokenizer::GetToken() {
    return token_;
}