#include "Slider.h"
#include <algorithm>

Slider::Slider(float sliderX, float sliderY, float width, float height, float thumbRadius)
    : _x(sliderX), _y(sliderY), _w(width), _h(height), _drag(false)
{
    // configure track
    _track.setSize({width, height});
    _track.setPosition(sliderX, sliderY);
    _track.setFillColor(sf::Color(155, 182, 109));  // #E0E0E0

    // configure thumb
    _thumb.setRadius(thumbRadius);
    _thumb.setOrigin(thumbRadius, thumbRadius);
    _thumb.setFillColor(sf::Color(0, 136, 78));   // #66BB6A
    setValue(0.5f);  // center thumb by default
}

void Slider::handleEvent(const sf::Event& e, const sf::RenderWindow& window) {
    if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
        auto p = window.mapPixelToCoords({e.mouseButton.x, e.mouseButton.y});
        if (_thumb.getGlobalBounds().contains(p)) {
            _drag = true;
        }
    }
    else if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left) {
        _drag = false;
    }
    else if (_drag && e.type == sf::Event::MouseMoved) {
        float mx = window.mapPixelToCoords({e.mouseMove.x, e.mouseMove.y}).x;
        mx = std::clamp(mx, _x, _x + _w);
        _thumb.setPosition(mx, _y + _h/2.f);
    }
}

void Slider::draw(sf::RenderWindow& window) const {
    window.draw(_track);
    window.draw(_thumb);
}

float Slider::getValue() const {
    return (_thumb.getPosition().x - _x) / _w;
}

void Slider::setValue(float value) {
    float v = std::clamp(value, 0.f, 1.f);
    float mx = _x + v * _w;
    _thumb.setPosition(mx, _y + _h/2.f);
}
