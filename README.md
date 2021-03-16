# FFT power spectrum visualizer

A high-quality demonstration of the application can be found at
- https://www.youtube.com/watch?v=AaJ25F8pjOw
- https://www.youtube.com/watch?v=HsuVb4TDCqU

![](data/output-opt.gif)

This program utilized discrete Fourier transformation to obtain the time-frequency-domain of a audio file. The result
can be explained as following:

- The top part of the program, we can see the raw time-amplitude data as it can be found in any `WAV` file. While this is the
  intuitive way for how we think of  data, it is not very informative.
- The bottom part of the program is the output of the algorithm, combined with some waterfall effect. This parts display the
  heart of DFT or Fourier Transformation in general, which allows us to decompose the data into their frequencies and how much each
  frequency contributes to the final sound. This kind of data has many nice attributes to which allow us to deploy many other tools
  from signal processing, machine learning and pattern recognition to wrangle data in a more effective way.

This program is on the other hand just a nice artistic demonstration of the algorithm, notice how much better we can see which piano key
is being pressed, which is almost impossible to achieve with time-amplitude.

### Build and run it yourself (C++11 / SFML 2.5)

Install the dependencies

```
sudo apt-get install libsfml-dev
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
- [Fourier transform for
  dummies](https://math.stackexchange.com/questions/1002/fourier-transform-for-dummies)
  
### Kyle's notes

```
brew install fmt cppunit sfml
```


```
rm -rfd build
mkdir build; cd build
cmake ..
make VERBOSE=1
./bin/main ../kyle-interview.wav
```

I tried using `CC=gcc-10 CXX=g++-10 cmake ..` and leaving`-fopenmp` in the root directory
`CMakeLists.txt`, but a single function from SFML was not found at link time. This is
likely because https://github.com/Homebrew/homebrew-core/blob/HEAD/Formula/sfml.rb was
compiled with Apple's Clang, not GCC?



### My Issue

https://github.com/longmakesstuff/FFT-Power-Spectrum/issues/1

Anything with a GUI is outside of my C++ wheelhouse, so I was hoping to play around with your very cool project to get some experience in the area. I also have limited CMake experience, so apologies for wasting your time if I am doing something silly. 

I am on macOS, so I installed `brew install fmt cppunit sfml` to get all the necessary dependencies for the project. Then,

```
mkdir build
cd build
CC=gcc-10 CXX=g++-10 cmake ..
```

But unfortunately that failed due to 
```
CMake Error at tests/CMakeLists.txt:1 (find_library):
  Could not find CPPUNIT using the following names:


-- Configuring incomplete, errors occurred!
```
I tried adding a [`Findcppunit.cmake`](https://raw.githubusercontent.com/rpavlik/cmake-modules/main/Findcppunit.cmake)  to `cmake_modules/` , but that did not avoid the error.  So I commented out `add_subdirectory(tests)` and CMake was able to finish. However, when I attempted `make`:

```
[ 14%] Linking CXX shared library libFFT-Spectrum.dylib
Undefined symbols for architecture x86_64:
  "__ZN2sf11SoundBuffer12loadFromFileERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE", referenced from:
      __ZN3fft3app10ControllerC2EPNS0_3GUIEyRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE in Controller.cpp.o
      __ZN3fft3app10ControllerC1EPNS0_3GUIEyRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE in Controller.cpp.o
...
```
Any tips?
-----------------------

Your recent changes on `master` indeed fixed the CPPUNIT error; thanks! But the `make` link error still happens for me.

I am on macOS, not Linux. But the development libraries for SFML are installed via Homebrew. E.g.
```
/usr/local/lib/cmake/SFML
/usr/local/lib/libsfml-audio.2.5.1.dylib
/usr/local/lib/libsfml-audio.2.5.dylib
/usr/local/lib/libsfml-audio.dylib
/usr/local/lib/libsfml-graphics.2.5.1.dylib
/usr/local/lib/libsfml-graphics.2.5.dylib
/usr/local/lib/libsfml-graphics.dylib
/usr/local/lib/libsfml-network.2.5.1.dylib
/usr/local/lib/libsfml-network.2.5.dylib
/usr/local/lib/libsfml-network.dylib
/usr/local/lib/libsfml-system.2.5.1.dylib
/usr/local/lib/libsfml-system.2.5.dylib
/usr/local/lib/libsfml-system.dylib
/usr/local/lib/libsfml-window.2.5.1.dylib
/usr/local/lib/libsfml-window.2.5.dylib
/usr/local/lib/libsfml-window.dylib
/usr/local/lib/pkgconfig/sfml-all.pc
/usr/local/lib/pkgconfig/sfml-audio.pc
/usr/local/lib/pkgconfig/sfml-graphics.pc
/usr/local/lib/pkgconfig/sfml-network.pc
/usr/local/lib/pkgconfig/sfml-system.pc
/usr/local/lib/pkgconfig/sfml-window.pc
/usr/local/share/SFML
...
```
all exist. Here is the complete error:

```
➜  build git:(master) ✗ make
[  0%] Built target copy-runtime-files
[ 10%] Linking CXX shared library libFFT-Spectrum.dylib
Undefined symbols for architecture x86_64:
  "__ZN2sf11SoundBuffer12loadFromFileERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE", referenced from:
      __ZN3fft3app10ControllerC2EPNS0_3GUIEyRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE in Controller.cpp.o
      __ZN3fft3app10ControllerC1EPNS0_3GUIEyRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE in Controller.cpp.o
  "__ZN2sf11SoundBufferC1Ev", referenced from:
      __ZN3fft3app10ControllerC2EPNS0_3GUIEyRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE in Controller.cpp.o
      __ZN3fft3app10ControllerC1EPNS0_3GUIEyRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE in Controller.cpp.o
  "__ZN2sf11SoundBufferD1Ev", referenced from:
      __ZN3fft3app10ControllerC2EPNS0_3GUIEyRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE.cold in Controller.cpp.o
      __ZN3fft3app10ControllerC1EPNS0_3GUIEyRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE.cold in Controller.cpp.o
  "__ZN2sf11VertexArrayC1ENS_13PrimitiveTypeEm", referenced from:
      __ZN3fft3app3GUIC1EPN2sf12RenderWindowEy in GUI.cpp.o
      __ZN3fft3app3GUIC2EPN2sf12RenderWindowEy in GUI.cpp.o
      __ZN3fft3app3GUI29visualize_history_frequenciesEv in GUI.cpp.o
  "__ZN2sf11VertexArrayixEm", referenced from:
      __ZN3fft3app3GUI19visualize_amplitudeEPKs._omp_fn.0 in GUI.cpp.o
      __ZN3fft3app3GUIC1EPN2sf12RenderWindowEy in GUI.cpp.o
      __ZN3fft3app3GUIC2EPN2sf12RenderWindowEy in GUI.cpp.o
      __ZN3fft3app3GUI29visualize_history_frequenciesEv in GUI.cpp.o
  "__ZN2sf12RenderStates7DefaultE", referenced from:
      __ZN3fft3app3GUI19visualize_amplitudeEPKs in GUI.cpp.o
      __ZN3fft3app3GUI29visualize_history_frequenciesEv in GUI.cpp.o
      __ZN3fft3app3GUI19visualize_frequencyEPKf in GUI.cpp.o
  "__ZN2sf12RenderTarget4drawERKNS_8DrawableERKNS_12RenderStatesE", referenced from:
      __ZN3fft3app3GUI19visualize_amplitudeEPKs in GUI.cpp.o
      __ZN3fft3app3GUI29visualize_history_frequenciesEv in GUI.cpp.o
      __ZN3fft3app3GUI19visualize_frequencyEPKf in GUI.cpp.o
  "__ZN2sf12RenderTarget5clearERKNS_5ColorE", referenced from:
      __ZN3fft3app3GUI5clearEv in GUI.cpp.o
  "__ZN2sf12RenderTarget7setViewERKNS_4ViewE", referenced from:
      __ZN3fft3app3GUI29visualize_history_frequenciesEv in GUI.cpp.o
  "__ZN2sf13Transformable11setPositionEff", referenced from:
      __ZN3fft3app3GUI29visualize_current_frequenciesEPKf in GUI.cpp.o
      __ZN3fft3app3GUIC1EPN2sf12RenderWindowEy in GUI.cpp.o
      __ZN3fft3app3GUIC2EPN2sf12RenderWindowEy in GUI.cpp.o
  "__ZN2sf14RectangleShape7setSizeERKNS_7Vector2IfEE", referenced from:
      __ZN3fft3app3GUI29visualize_current_frequenciesEPKf in GUI.cpp.o
      __ZN3fft3app3GUIC1EPN2sf12RenderWindowEy in GUI.cpp.o
      __ZN3fft3app3GUIC2EPN2sf12RenderWindowEy in GUI.cpp.o
  "__ZN2sf14RectangleShapeC1ERKNS_7Vector2IfEE", referenced from:
      __ZN3fft3app3GUIC1EPN2sf12RenderWindowEy in GUI.cpp.o
      __ZN3fft3app3GUIC2EPN2sf12RenderWindowEy in GUI.cpp.o
  "__ZN2sf4View4moveERKNS_7Vector2IfEE", referenced from:
      __ZN3fft3app3GUI29visualize_history_frequenciesEv in GUI.cpp.o
  "__ZN2sf5Color5BlackE", referenced from:
      __ZN3fft3app3GUI5clearEv in GUI.cpp.o
  "__ZN2sf5Color5WhiteE", referenced from:
      __ZN3fft3app3GUIC1EPN2sf12RenderWindowEy in GUI.cpp.o
      __ZN3fft3app3GUIC2EPN2sf12RenderWindowEy in GUI.cpp.o
  "__ZN2sf5ColorC1Ehhhh", referenced from:
      __ZN3fft3app3GUIC1EPN2sf12RenderWindowEy in GUI.cpp.o
      __ZN3fft3app3GUIC2EPN2sf12RenderWindowEy in GUI.cpp.o
      __ZN3fft3app3GUI15calculate_colorEv in GUI.cpp.o
  "__ZN2sf5ColorC1Ev", referenced from:
      __ZN3fft3app3GUIC1EPN2sf12RenderWindowEy in GUI.cpp.o
      __ZN3fft3app3GUIC2EPN2sf12RenderWindowEy in GUI.cpp.o
  "__ZN2sf5Shape12setFillColorERKNS_5ColorE", referenced from:
      __ZN3fft3app3GUIC1EPN2sf12RenderWindowEy in GUI.cpp.o
      __ZN3fft3app3GUIC2EPN2sf12RenderWindowEy in GUI.cpp.o
  "__ZN2sf5Sound4playEv", referenced from:
      __ZN3fft3app10Controller5startEv in Controller.cpp.o
  "__ZN2sf5Sound7setLoopEb", referenced from:
      __ZN3fft3app10ControllerC2EPNS0_3GUIEyRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE in Controller.cpp.o
      __ZN3fft3app10ControllerC1EPNS0_3GUIEyRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE in Controller.cpp.o
  "__ZN2sf5Sound9setBufferERKNS_11SoundBufferE", referenced from:
      __ZN3fft3app10ControllerC2EPNS0_3GUIEyRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE in Controller.cpp.o
      __ZN3fft3app10ControllerC1EPNS0_3GUIEyRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE in Controller.cpp.o
  "__ZN2sf5SoundC1Ev", referenced from:
      __ZN3fft3app10ControllerC2EPNS0_3GUIEyRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE in Controller.cpp.o
      __ZN3fft3app10ControllerC1EPNS0_3GUIEyRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE in Controller.cpp.o
  "__ZN2sf5SoundD1Ev", referenced from:
      __ZN3fft3app10ControllerC2EPNS0_3GUIEyRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE.cold in Controller.cpp.o
      __ZN3fft3app10ControllerC1EPNS0_3GUIEyRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE.cold in Controller.cpp.o
  "__ZN2sf6Window5closeEv", referenced from:
      __ZN3fft3app3GUI5closeEv in GUI.cpp.o
  "__ZN2sf6Window7displayEv", referenced from:
      __ZN3fft3app3GUI7displayEv in GUI.cpp.o
  "__ZN2sf6Window9pollEventERNS_5EventE", referenced from:
      __ZN3fft3app3GUI10poll_eventERN2sf5EventE in GUI.cpp.o
  "__ZNK2sf11SoundBuffer10getSamplesEv", referenced from:
      __ZN3fft4core12mean_channelERKN2sf11SoundBufferE in FFT.cpp.o
  "__ZNK2sf11SoundBuffer11getDurationEv", referenced from:
      __ZN3fft3app10ControllerC2EPNS0_3GUIEyRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE in Controller.cpp.o
      __ZN3fft3app10ControllerC1EPNS0_3GUIEyRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE in Controller.cpp.o
  "__ZNK2sf11SoundBuffer13getSampleRateEv", referenced from:
      __ZN3fft3app10ControllerC2EPNS0_3GUIEyRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE in Controller.cpp.o
      __ZN3fft3app10ControllerC1EPNS0_3GUIEyRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE in Controller.cpp.o
      __ZN3fft3app10Controller5startEv in Controller.cpp.o
  "__ZNK2sf11SoundBuffer14getSampleCountEv", referenced from:
      __ZN3fft3app10ControllerC2EPNS0_3GUIEyRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE in Controller.cpp.o
      __ZN3fft3app10ControllerC1EPNS0_3GUIEyRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE in Controller.cpp.o
      __ZN3fft4core12mean_channelERKN2sf11SoundBufferE in FFT.cpp.o
  "__ZNK2sf11SoundBuffer15getChannelCountEv", referenced from:
      __ZN3fft3app10ControllerC2EPNS0_3GUIEyRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE in Controller.cpp.o
      __ZN3fft3app10ControllerC1EPNS0_3GUIEyRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE in Controller.cpp.o
      __ZN3fft4core12mean_channelERKN2sf11SoundBufferE in FFT.cpp.o
  "__ZNK2sf11VertexArray14getVertexCountEv", referenced from:
      __ZN3fft3app3GUI19visualize_amplitudeEPKs._omp_fn.0 in GUI.cpp.o
      __ZN3fft3app3GUIC1EPN2sf12RenderWindowEy in GUI.cpp.o
      __ZN3fft3app3GUIC2EPN2sf12RenderWindowEy in GUI.cpp.o
  "__ZNK2sf12RenderTarget7getViewEv", referenced from:
      __ZN3fft3app3GUI29visualize_history_frequenciesEv in GUI.cpp.o
  "__ZNK2sf13Transformable11getPositionEv", referenced from:
      __ZN3fft3app3GUI29visualize_current_frequenciesEPKf in GUI.cpp.o
      __ZN3fft3app3GUIC1EPN2sf12RenderWindowEy in GUI.cpp.o
      __ZN3fft3app3GUIC2EPN2sf12RenderWindowEy in GUI.cpp.o
      __ZN3fft3app3GUI29visualize_history_frequenciesEv in GUI.cpp.o
  "__ZNK2sf14RectangleShape7getSizeEv", referenced from:
      __ZN3fft3app3GUI29visualize_current_frequenciesEPKf in GUI.cpp.o
  "__ZNK2sf4Time9asSecondsEv", referenced from:
      __ZN3fft3app10ControllerC2EPNS0_3GUIEyRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE in Controller.cpp.o
      __ZN3fft3app10ControllerC1EPNS0_3GUIEyRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE in Controller.cpp.o
      __ZN3fft3app10Controller5startEv in Controller.cpp.o
  "__ZNK2sf5Sound16getPlayingOffsetEv", referenced from:
      __ZN3fft3app10Controller5startEv in Controller.cpp.o
  "__ZNK2sf6Window6isOpenEv", referenced from:
      __ZN3fft3app3GUI7is_openEv in GUI.cpp.o
  "__ZTVN2sf11VertexArrayE", referenced from:
      __ZNSt6vectorIN2sf11VertexArrayESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_ in GUI.cpp.o
      __ZN3fft3app3GUI29visualize_history_frequenciesEv in GUI.cpp.o
      __ZN3fft3app3GUIC1EPN2sf12RenderWindowEy.cold in GUI.cpp.o
      __ZN3fft3app3GUIC2EPN2sf12RenderWindowEy.cold in GUI.cpp.o
  NOTE: a missing vtable usually means the first non-inline virtual member function has no definition.
ld: symbol(s) not found for architecture x86_64
collect2: error: ld returned 1 exit status
make[2]: *** [src/libFFT-Spectrum.dylib] Error 1
make[1]: *** [src/CMakeFiles/FFT-Spectrum.dir/all] Error 2
make: *** [all] Error 2

```
