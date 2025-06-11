#ifndef OPTIONSTHEME_H
#define OPTIONSTHEME_H

#include <string>
#include "Theme.h" // Importuje definicje struktur FontStyle i ButtonStyle

// ===================================================================================
//
//  PLIK DEFINIUJĄCY MOTYW GRAFICZNY DLA MENU OPCJI
//
//  Podobnie jak Theme.h, ten plik definiuje wygląd ekranu opcji.
//  W tym przypadku zamiast opisywania tła gradientem, wskazujemy bezpośrednio
//  na gotowy plik graficzny. Definiujemy tu również pozycje i wymiary
//  elementów specyficznych dla tego menu, jak suwaki.
//
// ===================================================================================

// Główna struktura opisująca motyw menu opcji.
struct OptionsTheme {
    // Ścieżka do gotowego obrazu tła dla menu opcji.
    std::string backgroundImagePath;

    // Styl głównego tytułu ("Opcje").
    FontStyle title;

    // Pozycje pionowe (Y) dla suwaków głośności.
    float sliderMusicY; // Pozycja suwaka muzyki
    float sliderSfxY;   // Pozycja suwaka efektów dźwiękowych

    // Wymiary suwaków.
    float sliderWidth;  // Szerokość ścieżki suwaka
    float sliderHeight; // Wysokość (grubość) ścieżki suwaka
    float thumbRadius;  // Promień kółka (uchwytu) suwaka

    // Pozycja pionowa (Y) dla przycisków "Zapisz" i "Anuluj".
    float saveButtonY;
    float cancelButtonY;

    // Style przycisków.
    ButtonStyle saveButton;
    ButtonStyle cancelButton;
};

// ===================================================================================
//  KONKRETNA IMPLEMENTACJA MOTYWU
// ===================================================================================
// Inicjalizacja stałej `OPTIONS_THEME` z gotowymi wartościami.
static const OptionsTheme OPTIONS_THEME = {
    // Ścieżka do tła
    "images/eco_tycoon_options.png",

    // Styl tytułu
    { "Comfortaa", 48, "#2E7D32", "bold", "center" },

    // Pozycje suwaków
    359.f,  // Y dla suwaka muzyki
    502.f,  // Y dla suwaka efektów

    // Wymiary suwaków
    490.f,  // Szerokość
    6.f,    // Grubość ścieżki
    15.f,   // Promień uchwytu

    // Pozycja Y przycisków
    580.f,
    580.f,

    // Styl przycisku "Zapisz"
    { {240, 70},
        {"#A5D6A7", "#66BB6A"},
        12,
        {"Comfortaa", 24, "#FFFFFF", "bold", "center"},
        16,
        {{"#81C784", "#4CAF50"}},
        {{"#66BB6A", "#388E3C"}}
    },

    // Styl przycisku "Anuluj" (w tym projekcie identyczny jak "Zapisz")
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
