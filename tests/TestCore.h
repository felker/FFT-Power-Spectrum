#ifndef CMAKE_EXAMPLE_TESTEXAMPLE_H
#define CMAKE_EXAMPLE_TESTEXAMPLE_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <core/FFT.h>

class TestCore : public CPPUNIT_NS::TestFixture {

CPPUNIT_TEST_SUITE(TestCore);
        CPPUNIT_TEST(test_channel_mean);
        CPPUNIT_TEST(test_hanning);
        CPPUNIT_TEST(test_fft);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp() override {
        TestFixture::setUp();
    }

    void tearDown() override {
        TestFixture::tearDown();
    }

    /**
     * Test the procedure core::mean_channel against Numpy's generated sample WAV file
     */
    static void test_channel_mean();

    /**
     * Compare the result of hanning coefficients with Numpy's hanning
     */
    static void test_hanning();

    /**
     * Sanity test
     */
    static void test_fft();
};


#endif
