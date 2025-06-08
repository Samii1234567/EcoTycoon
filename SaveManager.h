#pragma once

#include <string>
#include <vector>

// Zarządza katalogiem „saves/” i plikami zapisu
class SaveManager {
public:
    // Katalog, w którym trzymamy pliki .save
    static inline const std::string SAVE_DIR = "saves/";

    // Zapisuje pod nazwą `name` plik SAVE_DIR + name + ".save"
    // Zawartość: four floats: money energy maxEnergy environmentHealth
    static bool saveGame(const std::string& name,
                         float money,
                         float energy,
                         float maxEnergy,
                         float envHealth);

    // Zwraca listę dostępnych nazw zapisów (bez rozszerzenia)
    static std::vector<std::string> listSaves();

    // Wczytuje zapis `name` do podanych referencji; zwraca false przy błędzie
    static bool loadGame(const std::string& name,
                         float& money,
                         float& energy,
                         float& maxEnergy,
                         float& envHealth);
};
