# FFT power spectrum visualizer

A high-quality demonstration of the application can be found at https://www.youtube.com/watch?v=HsuVb4TDCqU.

![](data/output-opt.gif)

This program utilized discrete Fourier transformation to obtain the time-frequency-domain of a audio file. The result
can be explained as following:

- The top part of the program, we can see the raw amplitude-time data as it can be found in any `WAV` file.  
- The bottom part of the program is the output of the algorithm, combined with some waterfall effect.

### References
- Louisa Renz's bachelor thesis - University Tübingen
- [RosettaCode's Cooley-Tukey FFT](https://rosettacode.org/wiki/Fast_Fourier_transform)
- [Hann's function](https://en.wikipedia.org/wiki/Hann_function)
- [Fourier transform for dummies](https://math.stackexchange.com/questions/1002/fourier-transform-for-dummies)