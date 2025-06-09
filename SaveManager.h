#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <string>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Building.h" // Potrzebujemy definicji Building

// Zmodyfikowana struktura PlacedObject
struct PlacedObject {
    int typeId;
    sf::Sprite sprite;
    std::unique_ptr<Building> logic; // Używamy inteligentnego wskaźnika na bazową klasę
};

// Forward declaration
class Game;

class SaveManager {
public:
    static inline const std::string SAVE_DIR = "saves/";

    // Metody operują teraz na referencji do obiektu Game
    static bool saveGame(const std::string& name, const Game& game);
    static std::vector<std::string> listSaves();
    static bool loadGame(const std::string& name, Game& game, const std::vector<sf::Texture>* buildingTextures);
};

#endif // SAVEMANAGER_H
