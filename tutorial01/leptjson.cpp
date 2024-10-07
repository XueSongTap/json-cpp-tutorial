#include "leptjson.h"
#include <cctype>
#include <stdexcept>
#include <cassert>

namespace lept {

LeptValue::LeptValue() : type_(LeptType::Null) {}

LeptType LeptValue::getType() const {
    return type_;
}

void LeptValue::setType(LeptType type) {
    type_ = type;
}

ParseException::ParseException(ErrorCode code) : code_(code) {}

ErrorCode ParseException::code() const noexcept {
    return code_;
}

const char* ParseException::what() const noexcept {
    switch (code_) {
    case ErrorCode::Ok:
        return "Parse OK";
    case ErrorCode::ExpectValue:
        return "Expect value";
    case ErrorCode::InvalidValue:
        return "Invalid value";
    case ErrorCode::RootNotSingular:
        return "Root not singular";
    default:
        return "Unknown error";
    }
}

class Context {
public:
    explicit Context(const std::string& json) : json_(json), current_(0) {}

    void parseWhitespace();
    void parseNull(LeptValue& v);
    void parseValue(LeptValue& v);

    char peek() const;
    char get();
    void expect(char ch);

private:
    const std::string& json_;
    size_t current_;
};

void Context::parseWhitespace() {
    while (current_ < json_.size() && std::isspace(static_cast<unsigned char>(json_[current_]))) {
        ++current_;
    }
}

void Context::expect(char ch) {
    if (peek() != ch) {
        throw ParseException(ErrorCode::InvalidValue);
    }
    ++current_;
}

char Context::peek() const {
    if (current_ < json_.size()) {
        return json_[current_];
    } else {
        return '\0';
    }
}

char Context::get() {
    if (current_ < json_.size()) {
        return json_[current_++];
    } else {
        return '\0';
    }
}

void Context::parseNull(LeptValue& v) {
    expect('n');
    if (json_.compare(current_, 3, "ull") != 0) {
        throw ParseException(ErrorCode::InvalidValue);
    }
    current_ += 3;
    v.setType(LeptType::Null);
}

void Context::parseValue(LeptValue& v) {
    switch (peek()) {
    case 'n':
        parseNull(v);
        break;
    case '\0':
        throw ParseException(ErrorCode::ExpectValue);
    default:
        throw ParseException(ErrorCode::InvalidValue);
    }
}

void parse(LeptValue& v, const std::string& json) {
    Context c(json);
    c.parseWhitespace();
    c.parseValue(v);
    c.parseWhitespace();
    if (c.peek() != '\0') {
        throw ParseException(ErrorCode::RootNotSingular);
    }
}

} // namespace lept