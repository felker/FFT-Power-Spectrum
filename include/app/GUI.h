//
// Created by long on 12.03.21.
//

#ifndef FFT_SPECTRUM_GUI_H
#define FFT_SPECTRUM_GUI_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <memory>
#include "utils/Exception.h"
#include "utils/SizedVector.h"
#include "utils/Utils.h"

namespace fft {
    namespace app {
        class GUI {
        private:
            sf::RenderWindow *render_window;
            /*
             * This indicates how many raw amplitudes we want to visualize
             * We want of course to visualize them all
             */
            uint64_t amplitude_size;

            /*
             * Since the output of FFT is mirrored, we only want the first half
             *
             * The second half is basically the same. After log scaled the X-Axis,
             * using the whole output result into funny high frequencies at the right side
             * of the visualization. This bug was found by a Reddit user.
             *
             * This variable is simply half of amplitude_size
             */
            uint64_t frequency_size;

            /*
             * Represents the bins of each frequency
             */
            std::vector<sf::RectangleShape> frequency_bins;

            /*
             * Represents the cascading history_amplitude_lines
             */
            fft::utils::SizedVector<sf::VertexArray> history_amplitude_lines;

            /*
             * Represents the current window's time amplitude_lines
             */
            sf::VertexArray amplitude_lines;

            /*
             * Colors for gradients effect
             */
            sf::Color end_color{};
            sf::Color start_color{};
            bool increasing = true;
            uint64_t color_idx = 1;
            sf::Color current_color{};

            /*
             * List of colors to choose a random start color
             */
            std::vector<sf::Color> hot_colors{
                    {128, 0,   25, 50},
                    {220, 20,  60, 50},
                    {255, 215, 00, 50},
            };

            /*
             * List of colors to choose a random end color
             */
            std::vector<sf::Color> cold_colors{
                    {124, 252, 0,   50},
                    {0,   255, 255, 50},
                    {0,   255, 127, 50},
            };

            /**
             * Select randomly a new end color
             */
            void random_end_color();

            /**
             * Select randomly a new start color
             */
            void random_start_color();

            /**
             * Calculate the color in the middle
             */
            void calculate_color();

            /**
             * Plot the power spectrum of the current samples
             * @param spectrum DFT's power spectrum
             */
            void visualize_current_frequencies(const float *spectrum);

            /**
             * Plot the history_amplitude_lines
             */
            void visualize_history_frequencies();

        public:
            /**
             * Constructor
             * @param renderWindow pointer to a sf::RenderWindow
             * @param segment_size How many samples do we consider for one transformation
             */
            explicit GUI(sf::RenderWindow *renderWindow, uint64_t segment_size);

            /**
             * sf::RenderWindow's wrapper
             *
             * Close the GUI
             */
            void close();

            /**
             * sf::RenderWindow's wrapper
             *
             * @return true if the GUI is opened
             */
            bool is_open();

            /**
             * sf::RenderWindow's wrapper
             *
             * @param event event object to store the poll's result
             * @return true if event can be polled
             */
            bool poll_event(sf::Event &event);

            /**
             * Clear window
             */
            void clear();

            /**
             * Refresh window
             */
            void display();

            /**
             * Plot the power spectrum
             *
             * @param spectrum DFT's outputs
             * @param size how many coefficients
             */
            void visualize_frequency(const float *spectrum);

            /**
             * Plot the amplitude_lines
             *
             * @param amplitude_data current samples
             * @param size how many samples
             */
            void visualize_amplitude(const int16_t *amplitude_data);
        };
    }
}

#endif //FFT_SPECTRUM_GUI_H
