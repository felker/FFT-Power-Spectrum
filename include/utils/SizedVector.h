//
// Created by long on 12.03.21.
//

#ifndef FFT_SPECTRUM_SIZEDVECTOR_H
#define FFT_SPECTRUM_SIZEDVECTOR_H

#include <vector>
#include <cassert>
#include <cinttypes>
#include <stdexcept>
#include <ostream>
#include <set>

namespace fft {
    namespace utils {
        /**
         * A ring buffer style for saving incoming data
         *
         * @tparam T flexible template type
         */
        template<class T>
        class SizedVector {
        private:
            uint32_t cap;
            std::vector<T> mem;
        public:
            explicit SizedVector(uint32_t size) : cap(size) {}

            /**
             * Push a new element to the sized vector
             * @param t new variable
             * Also calculate the running mean
             */
            inline void push_back(const T &t) {
                // Delete first element
                if (this->mem.size() >= cap) {
                    this->mem.erase(mem.begin());
                }
                // Push new element
                this->mem.push_back(t);

                assert(mem.size() <= cap);
            }

            /**
             * Bulk push new data to vector
             * @param data  bulk data
             */
            inline void push_back(const std::vector<T> &data) {
                // There is free space
                if (data.size() < cap) {

                    for (size_t i = 0; i < data.size(); i++) {
                        this->push_back(data[i]);
                    }

                    // New data bulk's size is the same as the capacity
                } else if (data.size() == cap) {
                    this->reset();
                    for (size_t i = 0; i < data.size(); i++) {
                        this->push_back(data[i]);
                    }
                } else {
                    throw std::runtime_error("Can not handle data this big.");
                }

                assert(mem.size() <= cap);
            }

            inline size_t size() {
                return this->mem.size();
            }

            [[nodiscard]] inline uint32_t getCap() const {
                return cap;
            }

            inline void reset() {
                this->mem = std::vector<T>();
                this->set = std::set<T>();
            }

            inline std::vector<T> &data() {
                return mem;
            }

            inline T &operator[](uint32_t index) {
                return mem[index];
            }

            inline void operator<<(T t) {
                this->push_back(t);
            }

            friend std::ostream &operator<<(std::ostream &os, const SizedVector &vector) {
                os << "Sized Vector<" << vector.cap << "> [";
                for (const auto &i : vector.mem) {
                    os << i << ", ";
                }
                os << "]";
                os << std::endl;
                return os;
            }
        };
    }
}
#endif //FFT_SPECTRUM_SIZEDVECTOR_H
