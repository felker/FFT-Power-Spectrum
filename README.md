# FFT power spectrum visualizer

A high-quality demonstration of the application can be found at
- https://www.youtube.com/watch?v=AaJ25F8pjOw
- https://www.youtube.com/watch?v=HsuVb4TDCqU

![](data/output-opt.gif)

This program utilized discrete Fourier transformation to obtain the time-frequency-domain of a audio file. The result
can be explained as following:

- The top part of the program, we can see the raw amplitude-time data as it can be found in any `WAV` file. While this is the
  intuitive way for how we think of  data, it is not very informative.
- The bottom part of the program is the output of the algorithm, combined with some waterfall effect. This parts display the
  heart of DFT or Fourier Transformation in general, which allows us to decompose the data into their frequencies and how much each
  frequency contributes to the final sound. This kind of data has many nice attributes to which allow us to deploy many other tools
  from signal processing, machine learning and pattern recognition to wrangle data in a more effective way.

This program is on the other hand just a nice artistic demonstration of the algorithm, notice how much better we can see which piano key
is being pressed, which is almost impossible to achieve with time-amplitude.

### Build and run it yourself on Linux (For C++ people)

Install the dependencies

```
sudo apt-get install libcppunit-dev libsfml-dev
```

Clone the repository

```
git clone https://github.com/longmakesstuff/FFT-Power-Spectrum.git --depth 1
```

Compile

```
cd FFT-Power-Spectrum
mkdir build
cd build
cmake ..
make -j8
```

And play any `WAV` song you want with 

```
./bin/main [song-path.wav]
```

### References
- Louisa Renz's bachelor thesis - University Tübingen
- [RosettaCode's Cooley-Tukey FFT](https://rosettacode.org/wiki/Fast_Fourier_transform)
- [Hann's function](https://en.wikipedia.org/wiki/Hann_function)
- [Fourier transform for dummies](https://math.stackexchange.com/questions/1002/fourier-transform-for-dummies)