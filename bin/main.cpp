// Storage for audio samples defining a sound.
//
// A sound buffer holds the data of a sound, which is an array of audio samples.
//
// A sample is a 16 bits signed integer that defines the amplitude of the sound at a given time. The sound is then reconstituted by playing these samples at a high rate (for example, 44100 samples per second is the standard rate used for playing CDs).
// In short, audio samples are like texture pixels, and a sf::SoundBuffer is similar to a sf::Texture.
//
// A sound buffer can be loaded from a file (see loadFromFile() for the complete list of supported formats), from memory,
// from a custom stream (see sf::InputStream) or directly from an array of samples. It can also be saved back to a file.
//
// Sound buffers alone are not very useful: they hold the audio data but cannot be played. To do so, you need to use the sf::Sound class, which provides
// functions to play/pause/stop the sound as well as changing the way it is outputted (volume, pitch, 3D position, ...). This separation allows more flexibility and better performances:
// indeed a sf::SoundBuffer is a heavy resource, and any operation on it is slow (often too slow for real-time applications). On the other side, a sf::Sound is a lightweight object,
// which can use the audio data of a sound buffer and change the way it is played without actually modifying that data. Note that it is also possible to bind several sf::Sound instances to the same sf::SoundBuffer.
//
// It is important to note that the sf::Sound instance doesn't copy the buffer that it uses, it only keeps a reference to it. Thus, a sf::SoundBuffer must not be destructed while
// it is used by a sf::Sound (i.e. never write a function that uses a local sf::SoundBuffer instance for loading a sound).
#include <SFML/Graphics.hpp>
#include <app/Controller.h>
#include <app/GUI.h>


int main() {
    // Creating window
    sf::RenderWindow window({1900, 1000}, "FFT");

    window.setFramerateLimit(60);

    uint64_t segment_size = 1 << 13;

    fft::app::GUI gui(&window, segment_size);

    fft::app::Controller controller(&gui, segment_size);

    controller.start();
}