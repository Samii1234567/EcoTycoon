#ifndef GAMESTATE_H
#define GAMESTATE_H

// Ta definicja jest teraz w osobnym pliku, dostępnym dla całego projektu.
enum class GameState {
    MainMenu,
    EnterSaveName,
    LoadMenu,
    OptionsMenu,       // Dla opcji w menu głównym
    Playing,
    PauseMenu,
    EnergyMenu,
    InGameOptionsMenu  // NOWY STAN dla opcji w trakcie gry
};

#endif // GAMESTATE_H
