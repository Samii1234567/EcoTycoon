#include "SaveManager.h"
#include "Game.h"
#include "Constants.h"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

bool SaveManager::saveGame(const std::string& name, const Game& game) {
    fs::create_directories(SAVE_DIR);
    std::string path = SAVE_DIR + name + ".save";
    std::ofstream ofs(path, std::ios::trunc);
    if (!ofs) {
        std::cerr << "Nie można otworzyć pliku do zapisu: " << path << "\n";
        return false;
    }

    ofs << game.currentMoney << " "
        << game.currentEnergy << " "
        << game.maxEnergy << " "
        << game.environmentHealth << " "
        << game.totalGameTimeSeconds << "\n";

    ofs << game.placedObjects.size() << "\n";
    for (const auto& obj : game.placedObjects) {
        ofs << obj.typeId << " "
            << obj.gridPosition.x << " "
            << obj.gridPosition.y << " "
            << obj.price << " "
            << obj.level << "\n";
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
    if (!ifs) { return false; }

    game.reset();

    ifs >> game.currentMoney >> game.currentEnergy >> game.maxEnergy >> game.environmentHealth >> game.totalGameTimeSeconds;
    if (!ifs) return false;

    size_t numObjects;
    ifs >> numObjects;
    if (!ifs) return false;

    for (size_t i = 0; i < numObjects; ++i) {
        int typeId, gridX, gridY, price, level;
        ifs >> typeId >> gridX >> gridY >> price >> level;
        if (!ifs) { game.placedObjects.clear(); return false; }

        sf::Vector2i gridPos = {gridX, gridY};
        sf::Vector2f worldPos = game.getGrid().gridToWorldCoords(gridPos);

        game.placeBuilding(typeId, price, worldPos, false);
        if (!game.placedObjects.empty()) {
            game.placedObjects.back().level = level;
        }
    }
    game.currentSaveName = name;
    return true;
}
