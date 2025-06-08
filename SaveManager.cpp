#include "SaveManager.h"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

bool SaveManager::saveGame(const std::string& name,
                           float money,
                           float energy,
                           float maxEnergy,
                           float envHealth)
{
    // Upewnij się, że katalog istnieje
    fs::create_directories(SAVE_DIR);

    std::string path = SAVE_DIR + name + ".save";
    std::ofstream ofs(path, std::ios::trunc);
    if (!ofs) {
        std::cerr << "Nie można otworzyć pliku do zapisu: " << path << "\n";
        return false;
    }
    // Zapisz cztery liczby rozdzielone spacją
    ofs << money   << " "
        << energy  << " "
        << maxEnergy << " "
        << envHealth << "\n";
    return true;
}

std::vector<std::string> SaveManager::listSaves() {
    std::vector<std::string> names;
    if (!fs::exists(SAVE_DIR)) return names;
    for (auto& p : fs::directory_iterator(SAVE_DIR)) {
        if (p.path().extension() == ".save") {
            names.push_back(p.path().stem().string());
        }
    }
    return names;
}

bool SaveManager::loadGame(const std::string& name,
                           float& money,
                           float& energy,
                           float& maxEnergy,
                           float& envHealth)
{
    std::string path = SAVE_DIR + name + ".save";
    std::ifstream ifs(path);
    if (!ifs) {
        std::cerr << "Nie można otworzyć pliku do odczytu: " << path << "\n";
        return false;
    }
    ifs >> money >> energy >> maxEnergy >> envHealth;
    if (!ifs) {
        std::cerr << "Błąd odczytu danych z: " << path << "\n";
        return false;
    }
    return true;
}
