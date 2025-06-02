#ifndef OPTIONSTHEME_H
#define OPTIONSTHEME_H

#include <string>
#include "Theme.h"   // Defines FontStyle, ButtonStyle

// Theme for the Options menu, using a pre-rendered background image
struct OptionsTheme {
    // Path to the pre-generated options menu image
    std::string backgroundImagePath;
    // Title style ("Opcje")
    FontStyle title;
    // Y positions for slider placeholders under the labels
    float sliderMusicY;
    float sliderSfxY;
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
    300.f,
    420.f,
    // Buttons Y-coordinates
    580.f,
    580.f,
    // Save button style
    { {240, 70},           // size
        {"#A5D6A7", "#66BB6A"}, // gradient
        12,                   // cornerRadius
        {"Comfortaa", 24, "#FFFFFF", "bold", "center"}, // text
        16,                   // spacing
        {{"#81C784", "#4CAF50"}}, // hover gradient
        {{"#66BB6A", "#388E3C"}}  // active gradient
    },
    // Cancel button style (same as Save)
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
