#include "SaveManager.h"
#include "Game.h" // Potrzebna pełna definicja Game
#include "Constants.h"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

// Funkcja fabrykująca, tworząca odpowiedni obiekt budynku na podstawie ID
std::unique_ptr<Building> createBuildingById(int typeId) {
    switch (typeId) {
    case GameConstants::ENERGY_STORAGE_ID:
        return std::make_unique<EnergyStorage>();
    case GameConstants::SOLAR_PANEL_ID:
        return std::make_unique<SolarPanel>();
    case GameConstants::WIND_TURBINE_ID:
        return std::make_unique<WindTurbine>();
    default:
        std::cerr << "Nieznane ID budynku podczas tworzenia: " << typeId << std::endl;
        return nullptr;
    }
}


bool SaveManager::saveGame(const std::string& name, const Game& game) {
    fs::create_directories(SAVE_DIR);
    std::string path = SAVE_DIR + name + ".save";
    std::ofstream ofs(path, std::ios::trunc);
    if (!ofs) {
        std::cerr << "Nie można otworzyć pliku do zapisu: " << path << "\n";
        return false;
    }

    // Zapisz stan pobrany z obiektu game
    ofs << game.currentMoney << " "
        << game.currentEnergy << " "
        << game.maxEnergy << " "
        << game.environmentHealth << " "
        << game.totalGameTimeSeconds << "\n";

    // Zapisz obiekty
    ofs << game.placedObjects.size() << "\n";
    for (const auto& obj : game.placedObjects) {
        ofs << obj.typeId << " "
            << obj.sprite.getPosition().x << " "
            << obj.sprite.getPosition().y << "\n";
    }
    return true;
}

std::vector<std::string> SaveManager::listSaves() {
    std::vector<std::string> names;
    if (!fs::exists(SAVE_DIR)) return names;
    for (const auto& p : fs::directory_iterator(SAVE_DIR)) {
        if (p.path().extension() == ".save") {
            names.push_back(p.path().stem().string());
        }
    }
    return names;
}

bool SaveManager::loadGame(const std::string& name, Game& game, const std::vector<sf::Texture>* buildingTextures) {
    if (!buildingTextures) return false;

    std::string path = SAVE_DIR + name + ".save";
    std::ifstream ifs(path);
    if (!ifs) {
        std::cerr << "Nie można otworzyć pliku do odczytu: " << path << "\n";
        return false;
    }

    // Wyczyść stan gry przed wczytaniem
    game.reset();

    // Wczytaj dane do obiektu game
    ifs >> game.currentMoney >> game.currentEnergy >> game.maxEnergy >> game.environmentHealth >> game.totalGameTimeSeconds;
    if (!ifs) return false;

    size_t numObjects;
    ifs >> numObjects;
    if (!ifs) return false;

    for (size_t i = 0; i < numObjects; ++i) {
        int typeId;
        float posX, posY;
        ifs >> typeId >> posX >> posY;
        if (!ifs) { game.placedObjects.clear(); return false; }

        if (typeId < 0 || static_cast<size_t>(typeId) >= buildingTextures->size()) {
            game.placedObjects.clear(); return false;
        }

        PlacedObject newObj;
        newObj.typeId = typeId;
        newObj.logic = createBuildingById(typeId); // Użyj fabryki do stworzenia logiki
        if (!newObj.logic) { game.placedObjects.clear(); return false; }

        newObj.sprite.setTexture((*buildingTextures)[typeId]);
        auto texSize = newObj.sprite.getTexture()->getSize();
        newObj.sprite.setOrigin(texSize.x / 2.f, texSize.y / 2.f);
        newObj.sprite.setPosition(posX, posY);

        game.placedObjects.push_back(std::move(newObj));
    }

    return true;
}
