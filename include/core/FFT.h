//
// Created by long on 11.03.21.
//

#ifndef FFT_SPECTRUM_FFT_H
#define FFT_SPECTRUM_FFT_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <omp.h>
#include <numbers>
#include <cmath>
#include <valarray>
#include <complex>
#include "utils/Exception.h"
#include "utils/Utils.h"

namespace fft::core {
    typedef std::complex<double> Complex;

    /**
     *
     * Since we does not know how many channels a song can have, we can to calculate the arithmetric mean
     * between every channel's samples to run FFT on them.
     *
     * @param soundBuffer any sound buffer
     * @return the channels' mean of the samples
     * @tparam OutputType type of the output. We want to make this a bit more generic, could be int, float, etc.
     * @return
     */
    std::unique_ptr<int16_t []> mean_channel(const sf::SoundBuffer &soundBuffer);

    /**
     * Calculate and return the hanning coefficients to avoid leakage effect
     *
     * @tparam OutputType type of the returned buffer
     * @param size how many coefficients
     * @return hanning coefficients
     */
    std::unique_ptr<Complex []> hanning(uint32_t size);

    /**
     * Calculate the fourier coefficients of the samples
     * Author: https://rosettacode.org/wiki/Fast_Fourier_transform#C.2B.2B
     * Transform time spectrum into frequency spectrum
     *
     * @param x amplitude signal
     */
    void fft(Complex * x, uint32_t size);

    /**
     * Calculate the power spectrum
     *
     * @param array fourier coefficients
     * @param size size of array
     * @return the power spectrum
     */
    std::unique_ptr<float[]> power_spectrum(Complex * array, uint64_t size);
}

#endif //FFT_SPECTRUM_FFT_H
