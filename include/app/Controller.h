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
        sf::SoundBuffer sound_buffer;
        sf::Sound sound;
        fft::app::GUI *gui;
        sf::Event event{};
        std::unique_ptr<fft::core::Complex[]> mean_data;
        uint64_t channel_size;
        uint64_t segment_size;
    public:
        explicit Controller(GUI *gui, uint64_t segment_size);

        void start();
    };
}

#endif //FFT_SPECTRUM_CONTROLLER_H
