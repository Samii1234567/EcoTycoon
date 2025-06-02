#ifndef THEME_H
#define THEME_H

#include <vector>
#include <string>

// Proste struktury do trzymania parametrów UI
struct Size {
    float width;
    float height;
};

struct Gradient {
    std::vector<std::string> colors;
    std::string direction;
};

struct FontStyle {
    std::string fontFamily;
    int fontSize;
    std::string color;
    std::string weight;
    std::string align;
};

struct Shadow {
    int offsetX;
    int offsetY;
    int blur;
    std::string color;
};

struct Padding {
    int horizontal;
    int vertical;
};

struct ButtonStateGradient {
    std::vector<std::string> gradient;
};

struct ButtonStyle {
    Size size;
    std::vector<std::string> gradient;
    int cornerRadius;
    FontStyle text;
    int spacing;
    ButtonStateGradient hover;
    ButtonStateGradient active;
};

struct MainMenuTheme {
    Gradient background;
    FontStyle logo;
    std::string panelOverlayColor;
    int panelCornerRadius;
    Padding panelPadding;
    Shadow panelShadow;
    ButtonStyle button;
};

// Tutaj inicjalizujesz gotowy motyw dla menu głównego
static const MainMenuTheme MAIN_MENU_THEME = {
    // background
    { {"#E8F5E9", "#A5D6A7"}, "top-to-bottom" },
    // logo
    {"Comfortaa", 72, "#2E7D32", "bold", "center"},
    // panel overlay
    "rgba(255,255,255,0.4)",
    // panel corner radius
    16,
    // panel padding
    {32, 24},
    // panel shadow
    {0, 4, 12, "rgba(0,0,0,0.2)"},
    // button style
    {
        {405.f, 83.f},
        {"#A5D6A7", "#66BB6A"},
        12,
        {"Comfortaa", 24, "#FFFFFF", "bold", "center"},
        16,
        { {"#81C784", "#4CAF50"} },
        { {"#66BB6A", "#388E3C"} }
    }
};

#endif // THEME_H
