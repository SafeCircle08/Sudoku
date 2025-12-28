#pragma once

enum MousePrevState {
    UNCLICKED,
    CLICKED,
};

class Mouse {
private:
    static MousePrevState prevState;
public:
    static bool mouseLeftClicked();
    static bool mouseRightClicked();
    static bool mouseLeftPressed();
    static bool mouseRightPressed();
};