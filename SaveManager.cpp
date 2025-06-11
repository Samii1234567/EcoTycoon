#include "SaveManager.h"
#include "Game.h"
#include <filesystem> // Do operacji na plikach i folderach (C++17)
#include <fstream>    // Do strumieni plikowych (zapis/odczyt)
#include <iostream>

// Użycie przestrzeni nazw dla `std::filesystem` dla krótszego zapisu.
namespace fs = std::filesystem;

// ===================================================================================
//
//  PLIK IMPLEMENTACYJNY DLA KLASY SAVEMANAGER
//
//  Zawiera logikę metod do zapisu, odczytu, listowania i usuwania stanów gry.
//
// ===================================================================================

// Implementacja zapisu gry.
bool SaveManager::saveGame(const std::string& name, const Game& game) {
    // Upewniamy się, że folder "saves/" istnieje. Jeśli nie, tworzymy go.
    fs::create_directories(SAVE_DIR);

    // Tworzymy pełną ścieżkę do pliku zapisu (np. "saves/moja_gra.save").
    std::string path = SAVE_DIR + name + ".save";

    // Otwieramy strumień wyjściowy do pliku. `std::ios::trunc` usuwa poprzednią zawartość.
    std::ofstream ofs(path, std::ios::trunc);
    if (!ofs) {
        std::cerr << "Nie można otworzyć pliku do zapisu: " << path << "\n";
        return false;
    }

    // Zapisujemy główne statystyki gry w jednej linii.
    ofs << game.currentMoney << " "
        << game.currentEnergy << " "
        << game.maxEnergy << " "
        << game.environmentHealth << " "
        << game.totalGameTimeSeconds << "\n";

    // Zapisujemy liczbę postawionych obiektów.
    ofs << game.placedObjects.size() << "\n";
    // Iterujemy przez wszystkie obiekty i zapisujemy ich dane w osobnych liniach.
    for (const auto& obj : game.placedObjects) {
        ofs << obj.typeId << " "
            << obj.gridPosition.x << " "
            << obj.gridPosition.y << " "
            << obj.price << " "
            << obj.level << " "
            << obj.isDamaged << "\n";
    }
    return true; // Zapis udany.
}

// Implementacja listowania zapisów.
std::vector<std::string> SaveManager::listSaves() {
    std::vector<std::string> names;
    // Jeśli folder "saves/" nie istnieje, zwracamy pustą listę.
    if (!fs::exists(SAVE_DIR)) return names;

    // Iterujemy po wszystkich plikach w folderze "saves/".
    for (const auto& p : fs::directory_iterator(SAVE_DIR)) {
        // Interesują nas tylko pliki z rozszerzeniem ".save".
        if (p.path().extension() == ".save") {
            // Dodajemy do listy samą nazwę pliku, bez rozszerzenia.
            names.push_back(p.path().stem().string());
        }
    }
    return names;
}

// Implementacja wczytywania gry.
bool SaveManager::loadGame(const std::string& name, Game& game, const std::vector<sf::Texture>* buildingTextures) {
    if (!buildingTextures) return false;

    // Tworzymy pełną ścieżkę do pliku zapisu.
    std::string path = SAVE_DIR + name + ".save";
    std::ifstream ifs(path);
    if (!ifs) { return false; } // Jeśli plik nie istnieje lub nie można go otworzyć.

    // Resetujemy stan gry do wartości domyślnych przed wczytaniem nowych.
    game.reset();

    // Wczytujemy główne statystyki.
    ifs >> game.currentMoney >> game.currentEnergy >> game.maxEnergy >> game.environmentHealth >> game.totalGameTimeSeconds;
    if (!ifs) return false; // Sprawdzamy błędy strumienia.

    // Wczytujemy liczbę obiektów.
    size_t numObjects;
    ifs >> numObjects;
    if (!ifs) return false;

    // Pętla wczytująca dane każdego obiektu.
    for (size_t i = 0; i < numObjects; ++i) {
        int typeId, gridX, gridY, price, level;
        bool isDamaged;
        ifs >> typeId >> gridX >> gridY >> price >> level >> isDamaged;
        if (!ifs) { game.placedObjects.clear(); return false; } // W razie błędu czyścimy listę i przerywamy.

        // Odtwarzamy budynek za pomocą metody `placeBuilding`.
        sf::Vector2i gridPos = {gridX, gridY};
        sf::Vector2f worldPos = game.getGrid().gridToWorldCoords(gridPos);
        game.placeBuilding(typeId, price, worldPos, false); // `false`, bo nie jest to akcja gracza (nie odejmujemy pieniędzy).

        // Przywracamy dodatkowe dane, takie jak poziom i stan uszkodzenia.
        if (!game.placedObjects.empty()) {
            game.placedObjects.back().level = level;
            game.placedObjects.back().isDamaged = isDamaged;
            if(isDamaged) {
                // Zmieniamy kolor sprite'a na czerwony, jeśli budynek był uszkodzony.
                game.placedObjects.back().sprite.setColor(sf::Color(255, 100, 100));
            }
        }
    }
    // Zapisujemy nazwę wczytanej gry, aby kolejne zapisy mogły ją nadpisać.
    game.currentSaveName = name;
    return true; // Odczyt udany.
}

// Implementacja usuwania zapisu.
bool SaveManager::deleteGame(const std::string& name) {
    std::string path = SAVE_DIR + name + ".save";
    try {
        if (fs::exists(path)) {
            // Używamy `fs::remove` do usunięcia pliku.
            return fs::remove(path);
        }
    } catch (const fs::filesystem_error& e) {
        // Obsługa ewentualnych błędów systemu plików.
        std::cerr << "Błąd podczas usuwania pliku " << path << ": " << e.what() << '\n';
        return false;
    }
    return false; // Plik nie istniał.
}
