//
// Created by long on 11.03.21.
//

#include "core/FFT.h"

#define PI 3.14159265358979323846264338328L

namespace fft::core {
    ////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////
    // mean_channel
    ////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////
    std::unique_ptr<int16_t[]> mean_channel(const sf::SoundBuffer &soundBuffer) {

        uint64_t channel_size = soundBuffer.getSampleCount() / soundBuffer.getChannelCount();

        // Sanity check
        if (soundBuffer.getSampleCount() % soundBuffer.getChannelCount() != 0) {
            THROW_EXCEPTION("Channel count is not a divisor of sample count. Malicious data format!");
        }

        // Return buffer
        std::unique_ptr<int16_t []> ret = std::make_unique<int16_t[]>(channel_size);
        if (ret == nullptr) {
            THROW_EXCEPTION("Can not allocate enough memory for application to run. Please try again!")
        }

        // Sound's buffer
        const int16_t *sample = soundBuffer.getSamples();

        // How many channels are we dealing with
        const uint32_t channel_count = soundBuffer.getChannelCount();

#pragma omp parallel for
        for (uint64_t sample_idx = 0; sample_idx < channel_size; sample_idx++) {

            // The binary property of a computer number indicates, that the arithmetic mean
            // of any set of numbers can not be higher or lower than the biggest or smallest number in the set
            //
            // However the sum of them all together can result into overflow, therefore we add the sum of every channel's
            // sample at one moment into a 32-bit number
            int32_t sum = 0;

            for (uint32_t channel_idx = 0; channel_idx < channel_count; channel_idx++) {
                sum += sample[sample_idx * channel_count + channel_idx];
            }

            // Calculate the mean
            ret[sample_idx] = sum / channel_count;
        }

        return ret;
    }

    ////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////
    // fft
    ////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////
    void fft(Complex *x, uint32_t size) {
        unsigned int N = size, k = N, n;
        double thetaT = PI / N;
        Complex phiT = Complex(cos(thetaT), -sin(thetaT)), T;
        while (k > 1) {
            n = k;
            k >>= 1;
            phiT = phiT * phiT;
            T = 1.0L;
            for (unsigned int l = 0; l < k; l++) {
                for (unsigned int a = l; a < N; a += n) {
                    unsigned int b = a + k;
                    Complex t = x[a] - x[b];
                    x[a] += x[b];
                    x[b] = t * T;
                }
                T *= phiT;
            }
        }
        // Decimate
        auto m = (uint32_t) log2(N);
        for (unsigned int a = 0; a < N; a++) {
            unsigned int b = a;
            // Reverse bits
            b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
            b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
            b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
            b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
            b = ((b >> 16) | (b << 16)) >> (32 - m);
            if (b > a) {
                Complex t = x[a];
                x[a] = x[b];
                x[b] = t;
            }
        }
    }


    ////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////
    // hanning
    ////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////
    std::unique_ptr<Complex[]> hanning(uint32_t size) {
        std::unique_ptr<Complex[]> ret = std::make_unique<Complex[]>(size);

#pragma omp parallel for
        for (uint32_t i = 0; i < size; i++) {
            ret[i] = Complex(0.5 * (1 - std::cos(2 * PI * i / (size - 1))), 0.0);
        }

        return ret;
    }

    ////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////
    // power_spectrum
    ////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////
    std::unique_ptr<float[]> power_spectrum(Complex *array, uint64_t size) {
        std::unique_ptr<float[]> ret = std::make_unique<float[]>(size);

        for(uint64_t i = 0; i < size; i++) {
            ret[i] = std::sqrt(std::pow(array[i].real(), 2) + std::pow(array[i].imag(), 2));
        }

        return ret;
    }
}
