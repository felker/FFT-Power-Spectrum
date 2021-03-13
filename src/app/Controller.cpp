//
// Created by long on 12.03.21.
//

#include "app/Controller.h"

namespace fft::app {
    ////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////
    // Constructor
    ////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////
    Controller::Controller(GUI *gui, uint64_t segment_size, const std::string& file_name) : gui(gui), segment_size(segment_size) {
        // Load sound file into buffer

        fft::utils::TimeIt loading_sound("Loading sound from buffer");
        if (!this->sound_buffer.loadFromFile(file_name)) {
            std::cerr << "Can not load file" << file_name << ". Please place the file in the correct path." << std::endl;
            std::exit(1);
        }

        std::cout << this->sound_buffer << std::endl;
        //std::exit(0);

        this->sound.setBuffer(this->sound_buffer);
        loading_sound.end();

        this->channel_size = this->sound_buffer.getSampleCount() / this->sound_buffer.getChannelCount();

        // Centering the sound belong axis channels
        //
        // Resulting into 1D data, which we can comfortablely transform into complex numbers
        fft::utils::TimeIt centering_data("Centering data");
        auto mean = fft::core::mean_channel(this->sound_buffer);
        centering_data.end();

        // Transform sample data into complex number
        fft::utils::TimeIt making_complex("Making data complex");
        this->mean_data = std::make_unique<fft::core::Complex[]>(this->channel_size);

#pragma omp parallel for
        for (uint64_t i = 0; i < this->channel_size; i++) {
            this->mean_data[i] = fft::core::Complex(mean[i], 0.0);
        }
        making_complex.end();

        // Making the sound loop
        this->sound.setLoop(true);
    }

    ////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////
    // Main loop
    ////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////
    void Controller::start() {
        this->sound.play();
        while (this->gui->is_open()) {
            while (this->gui->poll_event(event)) {
                switch (this->event.type) {
                    case sf::Event::Closed:
                        this->gui->close();
                        break;
                    default:
                        break;
                }
            }
            this->gui->clear();

            /*
             * We want to pick the played segment in the last time delta
             * and visualize_frequency_domain it.
             *
             * This process is a random variable, since we don't know how many ms each segment may take.
             * So we have to do all the calculation live
             */
            // Obtaining the current sound offset
            double now = this->sound.getPlayingOffset().asSeconds();

            // Obtaining the start and the stop of the played segment
            uint64_t stop = now * this->sound_buffer.getSampleRate();
            if(stop < this->segment_size) {
                continue;
            }
            uint64_t start = stop - this->segment_size;

            // Creating buffer for the played segment
            //
            // Copy data from the sound buffer into this buffer
            //
            // Since FFT only works for sample size of power 2^N, we need to do some padding
            // if the current data is not ideal
            fft::utils::TimeIt copying_segment("Copying playing segment");
            auto played_segment = std::make_unique<fft::core::Complex[]>(this->segment_size);
            auto played_segment_real = std::make_unique<int16_t []>(this->segment_size);
            for(uint64_t i = 0; i < this->segment_size; i++) {
                played_segment[i] = this->mean_data[start + i];
                played_segment_real[i] = this->mean_data[start + i].real();
            }
            copying_segment.end();

            // Calculate the hanning coefficients and perform hann smoothing
            //
            // OMP threading seem to create an overhead which is not worth it
            // Therefore we won't parallelize this loop
            fft::utils::TimeIt hanning_smoothing("Hanning smoothing");
            auto hanning_coefficients = fft::core::hanning(this->segment_size);
            for(uint64_t i = 0; i < this->segment_size; i++) {
                played_segment[i] *= hanning_coefficients[i];
                played_segment_real[i] *= hanning_coefficients[i].real();
            }
            hanning_smoothing.end();

            // Run FFT and power spectrum
            fft::utils::TimeIt fft("FFT transformation");
            fft::core::fft(played_segment.get(), this->segment_size);
            auto power_spectrum = fft::core::power_spectrum(played_segment.get(), this->segment_size);
            fft.end();

            fft::utils::TimeIt visualization("Visualization");
            // Time to visualize_frequency_domain the spectrum
            this->gui->visualize_frequency_domain(power_spectrum.get());
            this->gui->visualize_time_domain(played_segment_real.get());
            visualization.end();

            // Refresh mainPanel
            this->gui->display();
        }
    }
}
