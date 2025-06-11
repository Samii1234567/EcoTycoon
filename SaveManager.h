#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <string>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

class Game;

class SaveManager {
public:
    static inline const std::string SAVE_DIR = "saves/";
    static bool saveGame(const std::string& name, const Game& game);
    static std::vector<std::string> listSaves();
    static bool loadGame(const std::string& name, Game& game, const std::vector<sf::Texture>* buildingTextures);
    static bool deleteGame(const std::string& name);
};

#endif // SAVEMANAGER_H
