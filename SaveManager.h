#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <string>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

// Deklaracja wyprzedzająca, aby uniknąć cyklicznych zależności nagłówków.
class Game;

// ===================================================================================
//
//  PLIK NAGŁÓWKOWY DLA KLASY SAVEMANAGER
//
//  Definiuje klasę `SaveManager`, która jest statyczną klasą narzędziową
//  do obsługi zapisu i odczytu stanu gry. Dzięki temu, że jest statyczna,
//  nie trzeba tworzyć jej obiektu, aby z niej korzystać.
//
// ===================================================================================

class SaveManager {
public:
    // --- Publiczne stałe i metody statyczne ---

    // Stała przechowująca nazwę folderu z zapisami.
    static inline const std::string SAVE_DIR = "saves/";

    // Zapisuje aktualny stan gry do pliku o podanej nazwie.
    // Zwraca `true`, jeśli zapis się powiódł.
    static bool saveGame(const std::string& name, const Game& game);

    // Zwraca listę nazw wszystkich dostępnych plików zapisu.
    static std::vector<std::string> listSaves();

    // Wczytuje stan gry z pliku o podanej nazwie.
    // Modyfikuje obiekt `game`, przywracając go do zapisanego stanu.
    // Zwraca `true`, jeśli odczyt się powiódł.
    static bool loadGame(const std::string& name, Game& game, const std::vector<sf::Texture>* buildingTextures);

    // Usuwa plik zapisu o podanej nazwie.
    // Zwraca `true`, jeśli usunięcie się powiodło.
    static bool deleteGame(const std::string& name);
};

#endif // SAVEMANAGER_H
