#ifndef OPTIONSTHEME_H
#define OPTIONSTHEME_H

#include <string>
#include "Theme.h"   // Defines FontStyle, ButtonStyle

// Theme for the Options menu, using a pre-rendered background image and slider config
struct OptionsTheme {
    // Path to the pre-generated options menu image
    std::string backgroundImagePath;
    // Title style ("Opcje")
    FontStyle title;
    // Y positions for slider placeholders under the labels
    float sliderMusicY;
    float sliderSfxY;
    // Slider dimensions and thumb radius
    float sliderWidth;
    float sliderHeight;
    float thumbRadius;
    // Y positions for the Save and Cancel buttons
    float saveButtonY;
    float cancelButtonY;
    // Button styles for Save and Cancel
    ButtonStyle saveButton;
    ButtonStyle cancelButton;
};

static const OptionsTheme OPTIONS_THEME = {
    // Background image path
    "images/eco_tycoon_options.png",
    // Title: "Opcje"
    { "Comfortaa", 48, "#2E7D32", "bold", "center" },
    // Slider placeholders (Y-coordinates)
    359.f,  // music slider track Y
    502.f,  // sfx slider track Y
    // Slider dimensions
    490.f,  // sliderWidth
    6.f,    // sliderHeight
    15.f,   // thumbRadius
    // Buttons Y-coordinates
    580.f,  // saveButtonY
    580.f,  // cancelButtonY
    // Save button style
    { {240, 70},                  // size
        {"#A5D6A7", "#66BB6A"}, // gradient
        12,                         // cornerRadius
        {"Comfortaa", 24, "#FFFFFF", "bold", "center"}, // text
        16,                         // spacing
        {{"#81C784", "#4CAF50"}}, // hover gradient
        {{"#66BB6A", "#388E3C"}}  // active gradient
    },
    // Cancel button style
    { {240, 70},
        {"#A5D6A7", "#66BB6A"},
        12,
        {"Comfortaa", 24, "#FFFFFF", "bold", "center"},
        16,
        {{"#81C784", "#4CAF50"}},
        {{"#66BB6A", "#388E3C"}}
    }
};

#endif // OPTIONSTHEME_H
