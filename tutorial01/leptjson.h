#ifndef LEPTJSON_H__
#define LEPTJSON_H__

#include <string>
#include <exception>

namespace lept {

enum class LeptType { Null, False, True, Number, String, Array, Object };

enum class ErrorCode {
    Ok = 0,
    ExpectValue,
    InvalidValue,
    RootNotSingular
};

class LeptValue {
public:
    LeptValue();
    LeptType getType() const;
    void setType(LeptType type);

private:
    LeptType type_;
};

class ParseException : public std::exception {
public:
    explicit ParseException(ErrorCode code);
    ErrorCode code() const noexcept;
    const char* what() const noexcept override;

private:
    ErrorCode code_;
};

void parse(LeptValue& v, const std::string& json);

} // namespace lept

#endif /* LEPTJSON_H__ */