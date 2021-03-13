//
// Created by long on 12.03.21.
//

#include <cmath>
#include "app/GUI.h"

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
fft::app::GUI::GUI(sf::RenderWindow *renderWindow, uint64_t segment_size) : render_window(renderWindow),
                                                                            amplitude_size(segment_size),
                                                                            frequency_size(segment_size / 2),
                                                                            frequency_bins(frequency_size),
                                                                            history_amplitude_lines(250),
                                                                            amplitude_lines(sf::VertexArray(sf::PrimitiveType::LineStrip, segment_size)){
    /*
     * Pre-initialize the power bins so we do not have to that every loop
     */
    for (uint64_t i = 0; i < this->frequency_size - 1; i++) {
        // We add + 1 in the log to avoid log(0)
        //
        // Log-scale the position of the bin and make it pass to the window's size
        this->frequency_bins[i].setPosition(std::log10((float)i + 1) / std::log10((float)this->frequency_size) * this->render_window->getSize().x,
                                            this->render_window->getSize().y);

        // We add + 2 in the log to avoid log(0)
        //
        // Log-scale the position of the bin and make it pass to the window's size
        this->frequency_bins[i + 1].setPosition(std::log10((float)i + 2) / std::log10((float)this->frequency_size) * this->render_window->getSize().x,
                                                this->render_window->getSize().y);

        // Set size of the bin from the beginning position of current i to the beginning position of i + 1
        this->frequency_bins[i].setSize({this->frequency_bins[i + 1].getPosition().x - this->frequency_bins[i ].getPosition().x, 0});
        this->frequency_bins[i].setFillColor(sf::Color::White);
    }

    /*
     * Pre initialize time domain line
     */
    for(uint64_t i = 0; i < this->amplitude_lines.getVertexCount(); i++) {
        this->amplitude_lines[i].color = sf::Color{225, 225, 255, 255};
    }

    this->random_end_color();
    this->random_start_color();
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// random_end_color
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
void fft::app::GUI::random_end_color() {
    this->end_color = *select_randomly(this->cold_colors.begin(), this->cold_colors.end());
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// random_start_color
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
void fft::app::GUI::random_start_color() {
    this->start_color = *select_randomly(this->hot_colors.begin(), this->hot_colors.end());
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// close
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
void fft::app::GUI::close() {
    this->render_window->close();
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// is_open
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
bool fft::app::GUI::is_open() {
    return this->render_window->isOpen();
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// poll_event
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
bool fft::app::GUI::poll_event(sf::Event &event) {
    return this->render_window->pollEvent(event);
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// clear
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
void fft::app::GUI::clear() {
    // Drawing white background
    this->render_window->clear(sf::Color::Black);
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// display
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
void fft::app::GUI::display() {
    this->render_window->display();
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// visualize_frequency
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
void fft::app::GUI::visualize_frequency(const float *spectrum) {
    this->visualize_current_frequencies(spectrum);

    this->visualize_history_frequencies();

    // Plot the current at last so the history does not overlap with current
    for (uint64_t i = 0; i < this->frequency_size; i++) {
        this->render_window->draw(frequency_bins[i]);
    }
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// visualize_current_frequencies
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
void fft::app::GUI::visualize_current_frequencies(const float *spectrum) {
    /*
     * We want to do a min-max scaling with the calculated power coefficients
     * For this purpose we need to find the most frequent frequency
     */
    float max = spectrum[0];
    for (uint64_t i = 0; i < this->frequency_size; i++) {
        if (spectrum[i] > max) {
            max = spectrum[i];
        }
    }

    /*
     * We do log scale on the X-Axis since human ears work on log scale (the same working principle of the unit decibel)
     */
    for (uint64_t i = 0; i < this->frequency_size; i++) {
        // Calculate the hight of the bin
        float bins_scaled_height = (spectrum[i] / max) * (this->render_window->getSize().y / 10.0f);

        // Set the y position of the bin according to the bins_scaled_height of bin
        // Keep the x coordinate
        this->frequency_bins[i].setPosition(this->frequency_bins[i].getPosition().x, this->render_window->getSize().y - bins_scaled_height);

        // Set the bins_scaled_height of the bin
        // Keep the width
        this->frequency_bins[i].setSize({this->frequency_bins[i].getSize().x, bins_scaled_height});

    }
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// visualize_history_frequencies
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
void fft::app::GUI::visualize_history_frequencies() {
    this->calculate_color();

    // Creating newest history_amplitude_lines
    sf::VertexArray line(sf::PrimitiveType::LineStrip, this->frequency_size);
    for (uint64_t i = 0; i < frequency_size; i++) {
        line[i].position = frequency_bins[i].getPosition();
        line[i].color = this->current_color;
    }
    this->history_amplitude_lines.push_back(line);

    // Drawing every history_amplitude_lines line
    sf::View original_view = this->render_window->getView();

    for(uint64_t i = 0; i < this->history_amplitude_lines.size(); i++) {
        sf::View history_view(original_view);
        history_view.move({0, (float)(i * 2.5)});
        this->render_window->setView(history_view);
        this->render_window->draw(this->history_amplitude_lines[this->history_amplitude_lines.size() - 1 - i]);
        this->render_window->setView(original_view);
    }

}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// visualize_amplitude
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
void fft::app::GUI::visualize_amplitude(const int16_t *amplitude_data) {
    float max = std::fabs(amplitude_data[0]);
    for (uint64_t i = 0; i < amplitude_size; i++) {
        if (std::fabs(amplitude_data[i]) > max) {
            max = std::fabs(amplitude_data[i]);
        }
    }
#pragma omp parallel for
    for(uint64_t i = 0; i < this->amplitude_lines.getVertexCount() ; i++) {
        this->amplitude_lines[i].position.x = ((float)i / (float)this->amplitude_lines.getVertexCount()) * this->render_window->getSize().x;
        this->amplitude_lines[i].position.y = 150 + ((float)amplitude_data[i] / max) * 50.0;
    }
    this->render_window->draw(amplitude_lines);
}


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// calculate_color
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
void fft::app::GUI::calculate_color() {
    // Calculate the newest history_amplitude_lines's color
    //
    // Simple interpolate new color between the start and the end color
    double percent = (double)this->color_idx / (double)this->history_amplitude_lines.getCap();
    uint8_t resultRed = this->start_color.r + percent * (this->end_color.r - this->start_color.r);
    uint8_t resultGreen = this->start_color.g + percent * (this->end_color.g - this->start_color.g);
    uint8_t resultBlue = this->start_color.b + percent * (this->end_color.b - this->start_color.b);
    this->current_color = sf::Color{resultRed, resultGreen, resultBlue, this->end_color.a};
    if(this->increasing) {
        this->color_idx++;
    }else {
        this->color_idx--;
    }

    // If the color reached its end, we want to switch other colors
    if(this->color_idx == this->history_amplitude_lines.getCap()) {
        this->increasing = false;
        this->random_start_color();
    } else if(this->color_idx == 0) {
        this->increasing = true;
        this->random_end_color();
    }
}
