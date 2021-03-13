#pragma once

#include <utility>
#include <exception>
#include <string>
#include <sstream>

namespace fft {
    namespace utils {
        class Exception : public std::exception {
        private:
            std::string message_;
        public:
            explicit Exception(std::string message) : message_(std::move(message)) {}

            [[nodiscard]] const char *what() const noexcept override {
                return message_.c_str();
            }
        };
    }
}

#define THROW_EXCEPTION(message) {std::stringstream __ss__; __ss__ << "\"" << message << "\" - " << __FILE__ << ":" << __LINE__ << std::endl; throw fft::utils::Exception(__ss__.str());};
