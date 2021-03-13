#ifndef CMAKE_EXAMPLE_TIMEIT_H
#define CMAKE_EXAMPLE_TIMEIT_H

#include <string>
#include <sstream>
#include <chrono>
#include <iostream>
#include <algorithm>

namespace fft {
    namespace utils {
        /**
         * Benchmarking class
         */
        class TimeIt {
        public:
            /**
             * Constructor
             * @param text name of the benchmark
             */
            explicit TimeIt(const std::string &text);

            /**
             * End the benchmark and print the result
             */
            void end();

            static std::string format_number(uint64_t number);

        private:
            static uint32_t recursion_level;
            static bool closed_last_line;
            bool closed_last_line_bkp{};
            std::chrono::steady_clock::time_point start_time;

            static void indent();
        };
    }
}

#endif
