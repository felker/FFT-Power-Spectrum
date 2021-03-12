//
// Created by long on 12.03.21.
//

#include <cmath>
#include "app/GUI.h"

fft::app::GUI::GUI(sf::RenderWindow *renderWindow, uint64_t segment_size) : renderWindow(renderWindow),
                                                                            segment_size(segment_size),
                                                                            power_bins(segment_size),
                                                                            history(250),
                                                                            time_domain(sf::VertexArray(sf::PrimitiveType::LineStrip, segment_size / 10)){
    /*
     * Pre-initialize the power bins so we do not have to that every loop
     */
    for (uint64_t i = 0; i < segment_size - 1; i++) {
        this->power_bins[i].setFillColor(sf::Color::White);
        this->power_bins[i].setPosition(std::log10((float)i) / std::log10(this->segment_size) * this->renderWindow->getSize().x,
                                  renderWindow->getSize().y);

        this->power_bins[i + 1].setFillColor(sf::Color::Black);
        this->power_bins[i + 1].setPosition(std::log10(i + 1) / std::log10(this->segment_size) * this->renderWindow->getSize().x,
                                      renderWindow->getSize().y);

        this->power_bins[i].setSize({this->power_bins[i + 1].getPosition().x - this->power_bins[i].getPosition().x, 0});
    }

    /*
     * Pre initialize time domain line
     */
    for(uint64_t i = 0; i < this->time_domain.getVertexCount(); i++) {
        this->time_domain[i].color = sf::Color{225, 225, 255, 255};
    }

    this->random_end_color();
    this->random_start_color();
}

void fft::app::GUI::random_end_color() {
    this->end_color = *select_randomly(this->cold_colors.begin(), this->cold_colors.end());
}

void fft::app::GUI::random_start_color() {
    this->start_color = *select_randomly(this->hot_colors.begin(), this->hot_colors.end());
}

void fft::app::GUI::close() {
    this->renderWindow->close();
}

bool fft::app::GUI::isOpen() {
    return this->renderWindow->isOpen();
}

bool fft::app::GUI::pollEvent(sf::Event &event) {
    return this->renderWindow->pollEvent(event);
}

void fft::app::GUI::clear() {
    // Drawing white backgroundß
    this->renderWindow->clear(sf::Color::Black);
}

void fft::app::GUI::display() {
    this->renderWindow->display();
}

void fft::app::GUI::visualize_frequency_domain(const float *spectrum, uint64_t size) {
    if (size == 0) {
        return;
    }
    this->visualize_history();

    this->visualize_bars(spectrum);
}

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
        float height = (spectrum[i] / max) * (this->renderWindow->getSize().y / 10.0f);

        this->power_bins[i].setPosition(this->power_bins[i].getPosition().x, this->renderWindow->getSize().y - height);
        this->power_bins[i].setSize({this->power_bins[i].getSize().x, height});

        this->renderWindow->draw(power_bins[i]);
    }
}

void fft::app::GUI::visualize_history() {
    // Calculate the newest history's color
    double percent = (double)this->current_color / (double)this->history.getCap();
    uint8_t resultRed = this->start_color.r + percent * (this->end_color.r - this->start_color.r);
    uint8_t resultGreen = this->start_color.g + percent * (this->end_color.g - this->start_color.g);
    uint8_t resultBlue = this->start_color.b + percent * (this->end_color.b - this->start_color.b);
    sf::Color color{resultRed, resultGreen, resultBlue, this->end_color.a};
    if(this->increasing) {
        this->current_color++;
    }else {
        this->current_color--;
    }
    if(this->current_color == this->history.getCap()) {
        this->increasing = false;
        this->random_start_color();
    } else if(this->current_color == 0) {
        this->increasing = true;
        this->random_end_color();
    }

    // Creating newest history
    sf::VertexArray line(sf::PrimitiveType::LineStrip, this->segment_size);
#pragma omp parallel for
    for (uint64_t i = 0; i < segment_size; i++) {
        line[i].position.x = power_bins[i].getPosition().x;
        line[i].position.y = power_bins[i].getPosition().y;
        line[i].color = color;
    }
    this->history.push_back(line);

    // Drawing every history line
    sf::View original_view = this->renderWindow->getView();

    for(uint64_t i = 0; i < this->history.size(); i++) {
        sf::View history_view;
        history_view.setCenter(original_view.getCenter());
        history_view.move({0, (float)(i * 2.5)});
        this->renderWindow->setView(history_view);
        this->renderWindow->draw(this->history[this->history.size() - 1 - i]);
        this->renderWindow->setView(original_view);
    }

}

void fft::app::GUI::visualize_time_domain(const int16_t *spectrum, uint64_t size) {
    float max = std::fabs(spectrum[0]);
    for (uint64_t i = 0; i < segment_size; i++) {
        if (std::fabs(spectrum[i]) > max) {
            max = std::fabs(spectrum[i]);
        }
    }
#pragma omp parallel for
    for(uint64_t i = 0; i < this->time_domain.getVertexCount() ; i++) {
        this->time_domain[i].position.x = ((float)i / (float)this->time_domain.getVertexCount()) * this->renderWindow->getSize().x;
        this->time_domain[i].position.y = 150 + (spectrum[i * 10] / max) * 50.0;
    }
    this->renderWindow->draw(time_domain);
}
