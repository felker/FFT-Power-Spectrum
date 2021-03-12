#pragma once
#include <utility>
#include <exception>
#include <string>
#include <sstream>

namespace fft::utils {
    class Exception : public std::exception {
    private:
        std::string message_;
    public:
        const char *what() const noexcept {
            return message_.c_str();
        }

        Exception(std::string message) : message_(std::move(message)) {}
    };
}

#define THROW_EXCEPTION(message) {std::stringstream __ss__; __ss__ << "\"" << message << "\" - " << __FILE__ << ":" << __LINE__ << std::endl; throw fft::utils::Exception(__ss__.str());};
