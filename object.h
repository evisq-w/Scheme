#pragma once

#include <memory>
#include <variant>

class Object : public std::enable_shared_from_this<Object> {
public:
    virtual ~Object() = default;
};

class Number : public Object {
public:
    Number(int value) : value_(value) {
    }

    int GetValue() const {
        return value_;
    }

private:
    int value_;
};

class Symbol : public Object {
public:
    Symbol(std::string str) : str_(str) {
    }

    const std::string& GetName() const {
        return str_;
    }

private:
    std::string str_;
};

class Cell : public Object {
public:
    Cell(std::shared_ptr<Object> a, std::shared_ptr<Object> b) : first_(a), second_(b) {
    }
    Cell(std::pair<std::shared_ptr<Object>, std::shared_ptr<Object>> pair)
        : first_(pair.first), second_(pair.second) {
    }

    std::shared_ptr<Object> GetFirst() const {
        return first_;
    }
    std::shared_ptr<Object> GetSecond() const {
        return second_;
    }

private:
    std::shared_ptr<Object> first_;
    std::shared_ptr<Object> second_;
};

///////////////////////////////////////////////////////////////////////////////

// Runtime type checking and conversion.
// This can be helpful: https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast

template <class T>
std::shared_ptr<T> As(const std::shared_ptr<Object>& obj) {
    return std::dynamic_pointer_cast<T>(obj);
}

template <class T>
bool Is(const std::shared_ptr<Object>& obj) {
    return std::dynamic_pointer_cast<T>(obj) != nullptr;
}