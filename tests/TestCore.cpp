#include "TestCore.h"

void TestCore::test_channel_mean() {
    sf::SoundBuffer soundBuffer;
    CPPUNIT_ASSERT_EQUAL(true, soundBuffer.loadFromFile("../../data/example.wav"));
    auto mean_buffer = fft::core::mean_channel(soundBuffer);

    CPPUNIT_ASSERT_EQUAL(true, mean_buffer != nullptr);

    // The example wav file was generated with value "2" on the first channel and value "4" on the second channel
    //
    // The mean of two channels therefore need to be "3"
    for (uint32_t i = 0; i < soundBuffer.getSampleCount() / soundBuffer.getChannelCount(); i++) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, mean_buffer[i], 0.0001);
    }

    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, 1, 0);
}

void TestCore::test_hanning() {
    constexpr uint32_t n = 10;
    auto hanning_buffer = fft::core::hanning(n);
    double expect[n]{0., 0.11697778, 0.41317591, 0.75, 0.96984631, 0.96984631, 0.75, 0.41317591, 0.11697778, 0.};
    for (uint32_t i = 0; i < n; i++) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expect[i], hanning_buffer[i].real(), 0.0001);
    }
}

void TestCore::test_fft() {
    constexpr uint32_t n = 8;
    fft::core::Complex test_data[n] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0};
    const double expected_real[n] = {28.0, -4.0, -4.0, -4.0, -4.0, -4.0, -4.0, -4.0};
    const double expected_img[n] = {0.0,9.65685424949238,4.0,1.6568542494923806,0.0,-1.6568542494923806,-4.0,-9.65685424949238};
    fft::core::fft(test_data, n);
    for (uint32_t i = 0; i < n; i++) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_img[i], test_data[i].imag(), 0.0001);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_real[i], test_data[i].real(), 0.0001);
    }
}
