//
// Created by long on 12.03.21.
//

#ifndef FFT_SPECTRUM_GUI_H
#define FFT_SPECTRUM_GUI_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "utils/Exception.h"
#include "utils/SizedVector.h"
#include "utils/Utils.h"

namespace fft::app {
    class GUI {
    private:
        sf::RenderWindow * render_window;
        uint64_t segment_size;

        /*
         * Represents the bins of each frequency
         */
        std::vector<sf::RectangleShape> power_bins;

        /*
         * Represents the cascading history
         */
        fft::utils::SizedVector<sf::VertexArray> history;

        /*
         * Represents the current window's time amplitude
         */
        sf::VertexArray amplitude;

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
                {128,0,25, 50},
                {220,20,60, 50},
                {255,215,00, 50},
        };

        /*
         * List of colors to choose a random end color
         */
        std::vector<sf::Color> cold_colors{
                {124,252,0, 50},
                {0,255,255, 50},
                {0,255,127, 50},
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
        void visualize_bars(const float *spectrum);

        /**
         * Plot the history
         */
        void visualize_history();
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
        bool poll_event(sf::Event& event);

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
        void visualize_frequency_domain(const float * spectrum);

        /**
         * Plot the amplitude
         *
         * @param samples current samples
         * @param size how many samples
         */
        void visualize_time_domain(const int16_t * samples);
    };
}

#endif //FFT_SPECTRUM_GUI_H
