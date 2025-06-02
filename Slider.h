#ifndef SLIDER_H
#define SLIDER_H

#include <SFML/Graphics.hpp>

// A draggable slider with a track and a thumb that returns values in [0,1]
class Slider {
public:
    // sliderX, sliderY – top-left of track
    // width, height     – size of track
    // thumbRadius       – radius of the draggable circle
    Slider(float sliderX, float sliderY, float width, float height, float thumbRadius);

    // Handle mouse down/up/move for dragging
    void handleEvent(const sf::Event& e, const sf::RenderWindow& window);

    // Draw track and thumb
    void draw(sf::RenderWindow& window) const;

    // Get normalized value (0.0–1.0)
    float getValue() const;

    // Set thumb position by normalized value (0.0–1.0)
    void setValue(float value);

private:
    sf::RectangleShape _track;
    sf::CircleShape    _thumb;
    float              _x, _y, _w, _h;
    bool               _drag = false;
};

#endif // SLIDER_H
