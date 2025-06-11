#ifndef THEME_H
#define THEME_H

#include <vector>
#include <string>

// ===================================================================================
//
//  PLIK DEFINIUJĄCY MOTYW GRAFICZNY DLA MENU GŁÓWNEGO
//
//  Ten plik zawiera zestaw struktur danych, które opisują wygląd różnych elementów
//  interfejsu użytkownika, takich jak kolory, czcionki, cienie i przyciski.
//  Umożliwia to łatwe zarządzanie spójnością wizualną aplikacji.
//
// ===================================================================================

// --- Podstawowe struktury pomocnicze ---

// Struktura przechowująca wymiary (szerokość i wysokość).
struct Size {
    float width;
    float height;
};

// Struktura opisująca gradient kolorów wraz z kierunkiem.
struct Gradient {
    std::vector<std::string> colors; // Lista kolorów w formacie hex (np. "#FFFFFF")
    std::string direction;           // Kierunek, np. "top-to-bottom"
};

// Struktura definiująca styl czcionki.
struct FontStyle {
    std::string fontFamily; // Nazwa rodziny czcionek
    int fontSize;           // Rozmiar w pikselach
    std::string color;      // Kolor tekstu
    std::string weight;     // Grubość czcionki (np. "bold")
    std::string align;      // Wyrównanie tekstu (np. "center")
};

// Struktura opisująca cień rzucany przez element UI.
struct Shadow {
    int offsetX;    // Przesunięcie w poziomie
    int offsetY;    // Przesunięcie w pionie
    int blur;       // Rozmycie cienia
    std::string color;  // Kolor cienia (może być z przezroczystością, np. "rgba(0,0,0,0.2)")
};

// Struktura definiująca wewnętrzne marginesy (padding).
struct Padding {
    int horizontal; // Margines poziomy
    int vertical;   // Margines pionowy
};

// Struktura pomocnicza do definiowania gradientu dla różnych stanów przycisku.
struct ButtonStateGradient {
    std::vector<std::string> gradient;
};

// --- Struktury złożone dla głównych komponentów ---

// Struktura kompleksowo opisująca styl przycisku.
struct ButtonStyle {
    Size size;                  // Wymiary przycisku
    std::vector<std::string> gradient; // Gradient dla stanu domyślnego
    int cornerRadius;           // Promień zaokrąglenia rogów
    FontStyle text;             // Styl tekstu na przycisku
    int spacing;                // Odstęp (nieużywane w tym projekcie, ale przydatne)
    ButtonStateGradient hover;  // Gradient dla stanu najechania myszą
    ButtonStateGradient active; // Gradient dla stanu aktywnego (wciśnięcia)
};

// Główna struktura łącząca wszystkie elementy motywu menu głównego.
struct MainMenuTheme {
    Gradient background;        // Motyw tła
    FontStyle logo;             // Styl tekstu logo
    std::string panelOverlayColor; // Kolor nakładki na panel (np. tło dla przycisków)
    int panelCornerRadius;      // Zaokrąglenie rogów panelu
    Padding panelPadding;       // Wewnętrzne marginesy panelu
    Shadow panelShadow;         // Cień panelu
    ButtonStyle button;         // Domyślny styl przycisków w menu
};

// ===================================================================================
//  KONKRETNA IMPLEMENTACJA MOTYWU
// ===================================================================================
// Inicjalizacja stałej `MAIN_MENU_THEME`, która przechowuje gotowe wartości
// dla motywu menu głównego. Zmiana wartości w tym miejscu wpłynie na wygląd gry.
static const MainMenuTheme MAIN_MENU_THEME = {
    // Tło (gradient od jasnozielonego do zielonego)
    { {"#E8F5E9", "#A5D6A7"}, "top-to-bottom" },
    // Logo ("Eco Tycoon")
    {"Comfortaa", 72, "#2E7D32", "bold", "center"},
    // Półprzezroczysta biała nakładka na panel z przyciskami
    "rgba(255,255,255,0.4)",
    // Zaokrąglenie rogów panelu
    16,
    // Wewnętrzne marginesy panelu
    {32, 24},
    // Lekki, czarny cień pod panelem
    {0, 4, 12, "rgba(0,0,0,0.2)"},
    // Styl przycisków
    {
        {405.f, 83.f},                  // Rozmiar przycisku
        {"#A5D6A7", "#66BB6A"},         // Domyślny gradient
        12,                             // Zaokrąglenie rogów
        {"Comfortaa", 24, "#FFFFFF", "bold", "center"}, // Biały, pogrubiony tekst
        16,                             // Spacing (nieużywany)
        { {"#81C784", "#4CAF50"} },     // Ciemniejszy gradient po najechaniu (hover)
        { {"#66BB6A", "#388E3C"} }      // Najciemniejszy gradient po wciśnięciu (active)
    }
};

#endif // THEME_H
