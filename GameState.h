#ifndef GAMESTATE_H
#define GAMESTATE_H

// Ta definicja jest teraz w osobnym pliku, dostępnym dla całego projektu.
enum class GameState {
    MainMenu,
    EnterSaveName,
    LoadMenu,
    OptionsMenu,
    Playing,
    PauseMenu,
    EnergyMenu
};

#endif // GAMESTATE_H
