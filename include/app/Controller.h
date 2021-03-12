//
// Created by long on 12.03.21.
//

#ifndef FFT_SPECTRUM_CONTROLLER_H
#define FFT_SPECTRUM_CONTROLLER_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "core/FFT.h"
#include "utils/TimeIt.h"
#include "GUI.h"

namespace fft::app {
    class Controller {
    private:
        /*
         * SFML's specific objects
         */
        sf::SoundBuffer sound_buffer;
        sf::Sound sound;
        fft::app::GUI *gui;
        sf::Event event{};

        /*
         * Sound data, but meaned belong channel axis
         */
        std::unique_ptr<fft::core::Complex[]> mean_data;

        /*
         * How many samples do each channel have
         */
        uint64_t channel_size;

        /*
         * How many samples do we consider in each transformation?
         */
        uint64_t segment_size;
    public:
        /**
         * Constructor
         *
         * @param gui pointer to fft::app::GUI
         * @param segment_size how many samples do we consider in each transformation
         */
        explicit Controller(GUI *gui, uint64_t segment_size);

        /**
         * Programm's main loop
         */
        void start();
    };
}

#endif //FFT_SPECTRUM_CONTROLLER_H
