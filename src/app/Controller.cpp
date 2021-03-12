//
// Created by long on 12.03.21.
//

#include "app/Controller.h"

namespace fft::app {
    Controller::Controller(GUI *gui, uint64_t segment_size) : gui(gui), segment_size(segment_size) {
        // Load sound file into buffer

        fft::utils::TimeIt loading_sound("Loading sound from buffer");
        if (!sound_buffer.loadFromFile("../../data/dido.wav")) {
            std::cerr << "Can not load file elise.wav. Please place the file in the correct path." << std::endl;
            std::exit(1);
        }

        std::cout << sound_buffer << std::endl;
        //std::exit(0);

        sound.setBuffer(sound_buffer);
        loading_sound.end();

        channel_size = sound_buffer.getSampleCount() / sound_buffer.getChannelCount();

        // Centering the sound belong axis channels
        //
        // Resulting into 1D data, which we can comfortablely transform into complex numbers
        fft::utils::TimeIt centering_data("Centering data");
        auto mean = fft::core::mean_channel(sound_buffer);
        centering_data.end();

        // Transform sample data into complex number
        fft::utils::TimeIt making_complex("Making data complex");
        mean_data = std::make_unique<fft::core::Complex[]>(channel_size);

#pragma omp parallel for
        for (uint64_t i = 0; i < channel_size; i++) {
            mean_data[i] = fft::core::Complex(mean[i], 0.0);
        }
        making_complex.end();

        // Making the sound loop
        sound.setLoop(true);
    }

    void Controller::start() {
        // Start to play sound
        sound.play();

        while (gui->isOpen()) {
            while (gui->pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::Closed:
                        gui->close();
                        break;
                    default:
                        break;
                }
            }
            gui->clear();

            /*
             * We want to pick the played segment in the last time delta
             * and visualize_frequency_domain it.
             *
             * This process is a random variable, since we don't know how many ms each segment may take.
             * So we have to do all the calculation live
             */
            // Obtaining the current sound offset
            double now = sound.getPlayingOffset().asSeconds();

            // Obtaining the start and the stop of the played segment
            uint64_t stop = now * sound_buffer.getSampleRate();
            if(stop < segment_size) {
                continue;
            }
            uint64_t start = stop - segment_size;

            // Creating buffer for the played segment
            //
            // Copy data from the sound buffer into this buffer
            //
            // Since FFT only works for sample size of power 2^N, we need to do some padding
            // if the current data is not ideal
            fft::utils::TimeIt copying_segment("Copying playing segment");
            auto played_segment = std::make_unique<fft::core::Complex[]>(segment_size);
            auto played_segment_real = std::make_unique<int16_t []>(segment_size);
            for(uint64_t i = 0; i < segment_size; i++) {
                played_segment[i] = mean_data[start + i];
                played_segment_real[i] = mean_data[start + i].real();
            }
            copying_segment.end();

            // Calculate the hanning coefficients and perform hann smoothing
            //
            // OMP threading seem to create an overhead which is not worth it
            // Therefore we won't parallelize this loop
            fft::utils::TimeIt hanning_smoothing("Hanning smoothing");
            auto hanning_coefficients = fft::core::hanning(segment_size);
            for(uint64_t i = 0; i < segment_size; i++) {
                played_segment[i] *= hanning_coefficients[i];
                played_segment_real[i] *= hanning_coefficients[i].real();
            }
            hanning_smoothing.end();

            // Run FFT and power spectrum
            fft::utils::TimeIt fft("FFT transformation");
            fft::core::fft(played_segment.get(), segment_size);
            auto power_spectrum = fft::core::power_spectrum(played_segment.get(), segment_size);
            fft.end();

            fft::utils::TimeIt visualization("Visualization");
            // Time to visualize_frequency_domain the spectrum
            this->gui->visualize_frequency_domain(power_spectrum.get(), segment_size);
            this->gui->visualize_time_domain(played_segment_real.get(), segment_size);
            visualization.end();

            // Refresh mainPanel
            gui->display();
        }
    }
}