#include "../header/Mouse.h"
#include <SFML/Graphics.hpp>

MousePrevState Mouse::prevState = UNCLICKED;

//--------------------MANAGING MOUSE CLICK--------------------
bool Mouse::mouseLeftClicked() {
    bool pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    if ((pressed) && (prevState == UNCLICKED)) {
        prevState = CLICKED;
        return true;
    } else if (pressed == false) {
        prevState = UNCLICKED;
    }
    return false;
}
bool Mouse::mouseRightClicked() {
    bool pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
    if (pressed && prevState == UNCLICKED) {
        prevState = CLICKED;
        return true;
    } else if (pressed == false) {
        prevState = UNCLICKED;
    }
    return false;
}

//--------------------MANAGING MOUSE PRESSED--------------------
bool Mouse::mouseLeftPressed() {
    bool pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    if (pressed) { return true; }
    return false;
}
bool Mouse::mouseRightPressed() {
    bool pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
    if (pressed) { return true; }
    return false;
}