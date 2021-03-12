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
        std::vector<sf::RectangleShape> power_bins;
        fft::utils::SizedVector<sf::VertexArray> history;
        sf::VertexArray time_domain;
        sf::Color end_color{};
        sf::Color start_color{};
        bool increasing = true;
        uint64_t color_idx = 1;
        sf::Color current_color{};

        /*
         *
         */
        std::vector<sf::Color> hot_colors{
                {128,0,25, 50},
                {220,20,60, 50},
                {255,215,00, 50},
        };

        /*
         *
         */
        std::vector<sf::Color> cold_colors{
                {124,252,0, 50},
                {0,255,255, 50},
                {0,255,127, 50},
        };

        /**
         *
         */
        void random_end_color();

        /**
         *
         */
        void random_start_color();

        /**
         *
         * @param spectrum
         */
        void visualize_bars(const float *spectrum);

        /**
         *
         */
        void visualize_history();

        /**
         *
         */
        void calculate_color();
    public:
        /**
         *
         * @param renderWindow
         * @param segment_size
         */
        explicit GUI(sf::RenderWindow *renderWindow, uint64_t segment_size);

        /**
         *
         */
        void close();

        /**
         *
         * @return
         */
        bool is_open();

        /**
         *
         * @param event
         * @return
         */
        bool poll_event(sf::Event& event);

        /**
         *
         */
        void clear();

        /**
         *
         */
        void display();

        /**
         *
         * @param spectrum
         * @param size
         */
        void visualize_frequency_domain(const float * spectrum, uint64_t size);

        /**
         *
         * @param samples
         * @param size
         */
        void visualize_time_domain(const int16_t * samples, uint64_t size);
    };
}

#endif //FFT_SPECTRUM_GUI_H
