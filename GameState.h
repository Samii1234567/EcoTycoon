#ifndef GAMESTATE_H
#define GAMESTATE_H

// ===================================================================================
//
//  PLIK NAGŁÓWKOWY DEFINIUJĄCY STANY GRY
//
//  Ten plik zawiera prosty typ wyliczeniowy `GameState`, który jest kluczowy
//  dla działania głównej pętli aplikacji (w pliku main.cpp).
//  Każda wartość reprezentuje inny ekran lub stan, w którym może znajdować się gra,
//  co pozwala na odpowiednie zarządzanie logiką, zdarzeniami i rysowaniem.
//
// ===================================================================================

enum class GameState {
    MainMenu,           // Ekran menu głównego
    EnterSaveName,      // Ekran wpisywania nazwy dla nowego zapisu gry
    LoadMenu,           // Ekran wczytywania zapisanej gry
    OptionsMenu,        // Ekran opcji dostępny z menu głównego
    Playing,            // Główny stan rozgrywki
    PauseMenu,          // Menu pauzy dostępne podczas gry
    EnergyMenu,         // Panel zarządzania (sprzedaży) energią
    InGameOptionsMenu,  // Panel opcji dostępny podczas gry
    ContractsMenu,      // Panel zarządzania kontraktami
    WeatherInfo         // Panel z informacjami o pogodzie
};

#endif // GAMESTATE_H
