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
                                                                            segment_size(segment_size),
                                                                            power_bins(segment_size),
                                                                            history(250),
                                                                            amplitude(sf::VertexArray(sf::PrimitiveType::LineStrip, segment_size)){
    /*
     * Pre-initialize the power bins so we do not have to that every loop
     */
    for (uint64_t i = 0; i < segment_size - 1; i++) {
        // We add + 1 in the log to avoid log(0)
        //
        // Log-scale the position of the bin and make it pass to the window's size
        this->power_bins[i].setPosition(std::log10((float)i + 1) / std::log10((float)this->segment_size) * this->render_window->getSize().x,
                                  this->render_window->getSize().y);

        // We add + 2 in the log to avoid log(0)
        //
        // Log-scale the position of the bin and make it pass to the window's size
        this->power_bins[i + 1].setPosition(std::log10((float)i + 2) / std::log10((float)this->segment_size) * this->render_window->getSize().x,
                                            this->render_window->getSize().y);

        // Set size of the bin from the beginning position of current i to the beginning position of i + 1
        this->power_bins[i].setSize({this->power_bins[i + 1].getPosition().x - this->power_bins[i ].getPosition().x, 0});
        this->power_bins[i].setFillColor(sf::Color::White);
    }

    /*
     * Pre initialize time domain line
     */
    for(uint64_t i = 0; i < this->amplitude.getVertexCount(); i++) {
        this->amplitude[i].color = sf::Color{225, 225, 255, 255};
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
// visualize_frequency_domain
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
void fft::app::GUI::visualize_frequency_domain(const float *spectrum, uint64_t size) {
    if (size == 0) {
        return;
    }

    this->visualize_bars(spectrum);

    this->visualize_history();

    for (uint64_t i = 0; i < this->segment_size; i++) {
        this->render_window->draw(power_bins[i]);
    }
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// visualize_bars
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
void fft::app::GUI::visualize_bars(const float *spectrum) {
    /*
     * We want to do a min-max scaling with the calculated power coefficients
     * For this purpose we need to find the most frequent frequency
     */
    float max = spectrum[0];
    for (uint64_t i = 0; i < this->segment_size; i++) {
        if (spectrum[i] > max) {
            max = spectrum[i];
        }
    }

    /*
     * We do log scale on the X-Axis since human ears work on log scale (the same working principle of the unit decibel)
     */
    for (uint64_t i = 0; i < this->segment_size; i++) {
        // Calculate the hight of the bin
        float bins_scaled_height = (spectrum[i] / max) * (this->render_window->getSize().y / 10.0f);

        // Set the y position of the bin according to the bins_scaled_height of bin
        // Keep the x coordinate
        this->power_bins[i].setPosition(this->power_bins[i].getPosition().x, this->render_window->getSize().y - bins_scaled_height);

        // Set the bins_scaled_height of the bin
        // Keep the width
        this->power_bins[i].setSize({this->power_bins[i].getSize().x, bins_scaled_height});

    }
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// visualize_history
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
void fft::app::GUI::visualize_history() {
    this->calculate_color();

    // Creating newest history
    sf::VertexArray line(sf::PrimitiveType::LineStrip, this->segment_size);
    for (uint64_t i = 0; i < segment_size; i++) {
        line[i].position = power_bins[i].getPosition();
        line[i].color = this->current_color;
    }
    this->history.push_back(line);

    // Drawing every history line
    sf::View original_view = this->render_window->getView();

    for(uint64_t i = 0; i < this->history.size(); i++) {
        sf::View history_view(original_view);
        history_view.setCenter(original_view.getCenter());
        history_view.move({0, (float)(i * 2.5)});
        this->render_window->setView(history_view);
        this->render_window->draw(this->history[this->history.size() - 1 - i]);
        this->render_window->setView(original_view);
    }

}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// visualize_time_domain
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
void fft::app::GUI::visualize_time_domain(const int16_t *spectrum, uint64_t size) {
    float max = std::fabs(spectrum[0]);
    for (uint64_t i = 0; i < segment_size; i++) {
        if (std::fabs(spectrum[i]) > max) {
            max = std::fabs(spectrum[i]);
        }
    }
#pragma omp parallel for
    for(uint64_t i = 0; i < this->amplitude.getVertexCount() ; i++) {
        this->amplitude[i].position.x = ((float)i / (float)this->amplitude.getVertexCount()) * this->render_window->getSize().x;
        this->amplitude[i].position.y = 150 + ((float)spectrum[i] / max) * 50.0;
    }
    this->render_window->draw(amplitude);
}


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// calculate_color
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
void fft::app::GUI::calculate_color() {
    // Calculate the newest history's color
    double percent = (double)this->color_idx / (double)this->history.getCap();
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
    if(this->color_idx == this->history.getCap()) {
        this->increasing = false;
        this->random_start_color();
    } else if(this->color_idx == 0) {
        this->increasing = true;
        this->random_end_color();
    }
}
