#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

#include "Theme.h"
#include "OptionsTheme.h"
#include "Slider.h"
#include "SaveManager.h"
#include "Game.h"
#include "Constants.h"
#include "AudioManager.h"

// ===================================================================================
//
//  GŁÓWNY PLIK APLIKACJI (PUNKT WEJŚCIOWY)
//
//  Ten plik zawiera funkcję `main`, która jest punktem startowym programu.
//  Odpowiada za:
//  1. Inicjalizację okna SFML.
//  2. Wczytanie wszystkich niezbędnych zasobów (czcionki, tekstury, dźwięki).
//  3. Stworzenie głównych obiektów gry (Game, AudioManager).
//  4. Uruchomienie głównej pętli aplikacji.
//  5. Zarządzanie maszyną stanów (przełączanie między menu, grą, pauzą itp.).
//
// ===================================================================================

int main() {
    // --- Inicjalizacja okna ---
    const unsigned WIN_W = 1200, WIN_H = 800;
    sf::RenderWindow window({WIN_W, WIN_H}, "Eco Tycoon", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true); // Włączenie V-Sync dla płynniejszego obrazu

    // --- Wczytywanie zasobów ---

    // Wczytanie głównej czcionki używanej w całej grze.
    sf::Font gameFont;
    if (!gameFont.loadFromFile("images/Quicksand-VariableFont_wght.ttf")) {
        std::cerr << "Nie można wczytać czcionki\n"; return -1;
    }

    // Inicjalizacja i wczytanie zasobów audio.
    AudioManager audioManager;
    if (!audioManager.loadAssets()) {
        std::cerr << "Wystapil blad podczas ladowania zasobow audio.\n";
    }

    // Wczytanie tekstur dla wszystkich typów budynków.
    std::vector<sf::Texture> buildingTextures(4);
    if (!buildingTextures[GameConstants::ENERGY_STORAGE_ID].loadFromFile("images/eco_storage.png") ||
        !buildingTextures[GameConstants::SOLAR_PANEL_ID].loadFromFile("images/eco_solar.png") ||
        !buildingTextures[GameConstants::WIND_TURBINE_ID].loadFromFile("images/eco_turbine.png") ||
        !buildingTextures[GameConstants::AIR_FILTER_ID].loadFromFile("images/eco_air_filter.png")) {
        std::cerr << "Nie mozna wczytac tekstur budynkow\n"; return -1;
    }

    // Wczytanie tekstur tła dla różnych menu.
    sf::Texture menuTex, optsTex, saveDialogTex, loadDialogTex;
    if (!menuTex.loadFromFile("images/eco_tycoon_menu.png") || !optsTex.loadFromFile(OPTIONS_THEME.backgroundImagePath) ||
        !saveDialogTex.loadFromFile("images/eco_tycoon_save.png") || !loadDialogTex.loadFromFile("images/eco_tycoon_load.png")) {
        return -1;
    }

    // --- Inicjalizacja obiektów gry i UI ---

    sf::Sprite menuSprite(menuTex), optsSprite(optsTex), saveDialogSprite(saveDialogTex), loadDialogSprite(loadDialogTex);
    sf::Vector2u windowSize = window.getSize();
    auto scaleSpriteToWindow = [&](sf::Sprite& sprite){
        sprite.setScale(windowSize.x / float(sprite.getTexture()->getSize().x), windowSize.y / float(sprite.getTexture()->getSize().y));
    };
    scaleSpriteToWindow(menuSprite); scaleSpriteToWindow(optsSprite); scaleSpriteToWindow(saveDialogSprite); scaleSpriteToWindow(loadDialogSprite);

    // Utworzenie suwaków dla menu opcji.
    Slider musicSlider((WIN_W - OPTIONS_THEME.sliderWidth)/2.f, OPTIONS_THEME.sliderMusicY, OPTIONS_THEME.sliderWidth, OPTIONS_THEME.sliderHeight, OPTIONS_THEME.thumbRadius);
    Slider sfxSlider((WIN_W - OPTIONS_THEME.sliderWidth)/2.f, OPTIONS_THEME.sliderSfxY, OPTIONS_THEME.sliderWidth, OPTIONS_THEME.sliderHeight, OPTIONS_THEME.thumbRadius);

    // Stworzenie głównego obiektu gry.
    Game game(gameFont, buildingTextures, audioManager);

    // Inicjalizacja elementów graficznych dla menu pauzy.
    sf::RectangleShape pauseOverlay({(float)WIN_W, (float)WIN_H}); pauseOverlay.setFillColor({0, 0, 0, 150});
    sf::Text pauseTitle("Pauza", gameFont, 60); pauseTitle.setFillColor(sf::Color::White); pauseTitle.setPosition((WIN_W - pauseTitle.getLocalBounds().width)/2.f, 150.f);
    sf::Text pauseResume("Wznow gre (P)", gameFont, 32); pauseResume.setFillColor(sf::Color::White); pauseResume.setPosition((WIN_W - pauseResume.getLocalBounds().width)/2.f, 280.f);
    sf::Text pauseSave("Zapisz gre", gameFont, 32); pauseSave.setFillColor(sf::Color::White); pauseSave.setPosition((WIN_W - pauseSave.getLocalBounds().width)/2.f, 330.f);
    sf::Text pauseWeatherInfo("Informacje o pogodzie", gameFont, 32); pauseWeatherInfo.setFillColor(sf::Color::White); pauseWeatherInfo.setPosition((WIN_W - pauseWeatherInfo.getLocalBounds().width)/2.f, 380.f);
    sf::Text pauseExit("Wyjdz do menu", gameFont, 32); pauseExit.setFillColor(sf::Color::White); pauseExit.setPosition((WIN_W - pauseExit.getLocalBounds().width)/2.f, 430.f);
    sf::Clock savedTextClock;
    bool showSavedText = false;

    // Inicjalizacja zmiennych dla ekranu wpisywania nazwy zapisu.
    std::string saveName;
    sf::Text inputPrompt("Nazwa zapisu:", gameFont, 34); inputPrompt.setFillColor({101, 67, 33}); inputPrompt.setPosition((WIN_W - inputPrompt.getLocalBounds().width) / 2.f, 186.f);
    sf::Text inputText("", gameFont, 40); inputText.setFillColor({101, 67, 33});
    sf::Clock cursorClock;

    // Inicjalizacja zmiennych dla ekranu wczytywania gry.
    std::vector<std::string> saves;
    size_t selectedIndex = 0;
    sf::FloatRect deleteButtonHotspot;

    // Ustawienie początkowego stanu gry i zegara delty.
    GameState state = GameState::MainMenu;
    sf::Clock deltaClock;

    // ===================================================================================
    //  GŁÓWNA PĘTLA APLIKACJI
    // ===================================================================================
    while (window.isOpen()) {
        // Logika pomocnicza, np. do ukrywania powiadomień po pewnym czasie.
        if (showSavedText && savedTextClock.getElapsedTime().asSeconds() > 1.5f) {
            showSavedText = false;
            pauseSave.setString("Zapisz gre");
            pauseSave.setPosition((WIN_W - pauseSave.getLocalBounds().width)/2.f, 330.f);
            pauseSave.setFillColor(sf::Color::White);
        }

        // Zarządzanie muzyką w zależności od stanu gry.
        bool inMenu = (state == GameState::MainMenu || state == GameState::OptionsMenu || state == GameState::EnterSaveName || state == GameState::LoadMenu);
        if (inMenu) {
            audioManager.playMenuMusic();
        } else {
            audioManager.playGameMusic();
        }

        // Aktualizacja głośności (wartości są mnożone przez 100, bo AudioManager oczekuje zakresu 0-100).
        audioManager.setMusicVolume(game.musicVolume * 100.f);
        audioManager.setSfxVolume(game.sfxVolume * 100.f);

        // --- PĘTLA OBSŁUGI ZDARZEŃ ---
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) window.close();

            // Przekazanie zdarzenia do obiektu `game`, jeśli jesteśmy w stanie rozgrywki.
            if (state == GameState::Playing || state == GameState::EnergyMenu || state == GameState::InGameOptionsMenu || state == GameState::ContractsMenu || state == GameState::WeatherInfo) {
                game.handleEvent(e, window, state);
            }

            // --- MASZYNA STANÓW - OBSŁUGA ZDARZEŃ ---
            // Logika specyficzna dla każdego stanu gry.
            switch (state) {
            case GameState::PauseMenu:
                if (e.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2f pos = window.mapPixelToCoords({e.mouseButton.x, e.mouseButton.y});
                    if (pauseResume.getGlobalBounds().contains(pos)) {
                        audioManager.play(SoundEffect::ButtonClick);
                        state = GameState::Playing;
                        deltaClock.restart(); // Reset delty czasu, by uniknąć "skoku" w logice gry.
                    } else if (pauseSave.getGlobalBounds().contains(pos) && !showSavedText) {
                        audioManager.play(SoundEffect::ButtonClick);
                        if (!game.currentSaveName.empty()) {
                            if(SaveManager::saveGame(game.currentSaveName, game)) {
                                pauseSave.setString("Zapisano!");
                                pauseSave.setFillColor(sf::Color::Green);
                                pauseSave.setPosition((WIN_W - pauseSave.getLocalBounds().width)/2.f, 330.f);
                                savedTextClock.restart();
                                showSavedText = true;
                            }
                        } else {
                            pauseSave.setString("Brak nazwy zapisu!");
                            pauseSave.setFillColor(sf::Color::Red);
                            pauseSave.setPosition((WIN_W - pauseSave.getLocalBounds().width)/2.f, 330.f);
                            savedTextClock.restart();
                            showSavedText = true;
                        }
                    } else if (pauseWeatherInfo.getGlobalBounds().contains(pos)) {
                        audioManager.play(SoundEffect::ButtonClick);
                        state = GameState::WeatherInfo;
                    } else if (pauseExit.getGlobalBounds().contains(pos)) {
                        audioManager.play(SoundEffect::ButtonClick);
                        if (!game.currentSaveName.empty()) SaveManager::saveGame(game.currentSaveName, game);
                        state = GameState::MainMenu;
                    }
                }
                if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::P) {
                    state = GameState::Playing;
                    deltaClock.restart();
                }
                break;
            case GameState::MainMenu:
                if (e.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2f pos = window.mapPixelToCoords({e.mouseButton.x, e.mouseButton.y});
                    // Definiowanie klikalnych obszarów dla przycisków w menu głównym.
                    const auto& btn = MAIN_MENU_THEME.button;
                    sf::FloatRect hotNew( (WIN_W - btn.size.width)/2.f, 353.f, btn.size.width, btn.size.height );
                    sf::FloatRect hotLoad(hotNew.left, hotNew.top + btn.size.height + 20.f, hotNew.width, hotNew.height);
                    sf::FloatRect hotOpt(hotNew.left, hotLoad.top + btn.size.height + 20.f, hotNew.width, hotNew.height);
                    sf::FloatRect hotExit(hotNew.left, hotOpt.top + btn.size.height + 20.f, hotNew.width, hotNew.height);

                    if (hotNew.contains(pos)) {
                        audioManager.play(SoundEffect::ButtonClick);
                        game.reset();
                        saveName.clear();
                        inputText.setString("");
                        cursorClock.restart();
                        state = GameState::EnterSaveName;
                    } else if (hotLoad.contains(pos)) {
                        audioManager.play(SoundEffect::ButtonClick);
                        saves = SaveManager::listSaves();
                        selectedIndex = 0;
                        state = GameState::LoadMenu;
                    } else if (hotOpt.contains(pos)) {
                        audioManager.play(SoundEffect::ButtonClick);
                        musicSlider.setValue(game.musicVolume);
                        sfxSlider.setValue(game.sfxVolume);
                        state = GameState::OptionsMenu;
                    } else if (hotExit.contains(pos)) {
                        audioManager.play(SoundEffect::ButtonClick);
                        window.close();
                    }
                }
                break;
            case GameState::LoadMenu:
                if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f pos = window.mapPixelToCoords({e.mouseButton.x, e.mouseButton.y});
                    if (deleteButtonHotspot.contains(pos)) {
                        if (!saves.empty()) {
                            audioManager.play(SoundEffect::ButtonClick);
                            SaveManager::deleteGame(saves[selectedIndex]);
                            saves = SaveManager::listSaves(); // Odświeżenie listy.
                            if (selectedIndex >= saves.size() && !saves.empty()) {
                                selectedIndex = saves.size() - 1;
                            }
                        }
                    } else {
                        float listAreaWidth = 482.f, listStartX = (WIN_W - listAreaWidth) / 2.f, listStartY = 270.f, lh = 40.f;
                        for (size_t i = 0; i < saves.size(); ++i) {
                            sf::FloatRect itemHotspot(listStartX, listStartY + i * lh, listAreaWidth, lh);
                            if (itemHotspot.contains(pos)) {
                                if (selectedIndex == i) { // Podwójne kliknięcie wczytuje grę.
                                    if (SaveManager::loadGame(saves[selectedIndex], game, &buildingTextures)) {
                                        state = GameState::Playing;
                                        deltaClock.restart();
                                    } else { std::cerr << "Wczytywanie nie powiodlo sie.\n"; }
                                } else { // Pojedyncze kliknięcie wybiera zapis.
                                    selectedIndex = i;
                                    audioManager.play(SoundEffect::ButtonClick);
                                }
                                break;
                            }
                        }
                    }
                }
                else if (e.type == sf::Event::KeyPressed) {
                    if (e.key.code == sf::Keyboard::Escape) state = GameState::MainMenu;
                    else if (e.key.code == sf::Keyboard::Up && selectedIndex > 0) --selectedIndex;
                    else if (e.key.code == sf::Keyboard::Down && selectedIndex + 1 < saves.size()) ++selectedIndex;
                    else if (e.key.code == sf::Keyboard::Enter && !saves.empty()) {
                        if (SaveManager::loadGame(saves[selectedIndex], game, &buildingTextures)) {
                            state = GameState::Playing;
                            deltaClock.restart();
                        } else { std::cerr << "Wczytywanie nie powiodlo sie.\n"; }
                    }
                }
                break;
            case GameState::EnterSaveName:
                if (e.type == sf::Event::TextEntered) {
                    if ((e.text.unicode == '\r' || e.text.unicode == '\n') && !saveName.empty()) {
                        game.currentSaveName = saveName;
                        SaveManager::saveGame(saveName, game);
                        state = GameState::Playing;
                        deltaClock.restart();
                    } else if (e.text.unicode == 8 && !saveName.empty()) { // Backspace
                        saveName.pop_back();
                    } else if (e.text.unicode >= 32 && e.text.unicode < 128) { // Zwykłe znaki ASCII
                        saveName.push_back(static_cast<char>(e.text.unicode));
                    }
                    inputText.setString(saveName);
                    inputText.setPosition((WIN_W - inputText.getLocalBounds().width) / 2.f, 290.f);
                }
                if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape) state = GameState::MainMenu;
                break;
            case GameState::OptionsMenu:
                musicSlider.handleEvent(e, window);
                sfxSlider.handleEvent(e, window);
                if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f pos = window.mapPixelToCoords({e.mouseButton.x, e.mouseButton.y});
                    const float buttonWidth = OPTIONS_THEME.saveButton.size.width, buttonHeight = OPTIONS_THEME.saveButton.size.height;
                    const float buttonY = OPTIONS_THEME.saveButtonY, gap = 40.f;
                    const float saveX = (WIN_W / 2.f) - buttonWidth - (gap / 2.f), cancelX = (WIN_W / 2.f) + (gap / 2.f);
                    sf::FloatRect saveHotspot(saveX, buttonY, buttonWidth, buttonHeight);
                    sf::FloatRect cancelHotspot(cancelX, buttonY, buttonWidth, buttonHeight);
                    if (saveHotspot.contains(pos)) {
                        audioManager.play(SoundEffect::ButtonClick);
                        game.musicVolume = musicSlider.getValue();
                        game.sfxVolume = sfxSlider.getValue();
                        state = GameState::MainMenu;
                    } else if (cancelHotspot.contains(pos)) {
                        audioManager.play(SoundEffect::ButtonClick);
                        state = GameState::MainMenu;
                    }
                }
                if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape) {
                    state = GameState::MainMenu;
                }
                break;
            default:
                break;
            }
        }

        // --- AKTUALIZACJA LOGIKI GRY ---
        // Logika gry jest aktualizowana tylko w stanie `Playing`.
        if (state == GameState::Playing) {
            game.update(deltaClock.restart().asSeconds());
        }

        // --- RYSOWANIE ---
        window.clear();

        // --- MASZYNA STANÓW - RYSOWANIE ---
        // Rysowanie odpowiedniej sceny w zależności od stanu gry.
        switch (state) {
        case GameState::Playing:
        case GameState::EnergyMenu:
        case GameState::InGameOptionsMenu:
        case GameState::ContractsMenu:
        case GameState::WeatherInfo:
            game.draw(window, state);
            break;
        case GameState::PauseMenu:
            game.draw(window, state); // Rysujemy grę w tle
            window.draw(pauseOverlay); // a na niej nakładkę pauzy.
            window.draw(pauseTitle);
            window.draw(pauseResume);
            window.draw(pauseSave);
            window.draw(pauseWeatherInfo);
            window.draw(pauseExit);
            break;
        case GameState::MainMenu: window.draw(menuSprite); break;
        case GameState::LoadMenu:
            window.draw(loadDialogSprite);
            { // Nowy zasięg, by uniknąć błędów "crosses initialization"
                deleteButtonHotspot = {};
                float listAreaWidth = 482.f, listStartX = (WIN_W - listAreaWidth) / 2.f, listStartY = 270.f, lh = 40.f;
                sf::Text deleteButton("X", gameFont, 24);
                deleteButton.setFillColor(sf::Color::Red);
                for (size_t i = 0; i < saves.size(); ++i) {
                    if (i == selectedIndex) {
                        sf::RectangleShape hl({listAreaWidth, lh - 4.f});
                        hl.setFillColor({170, 200, 255, 180});
                        hl.setPosition(listStartX, listStartY + i * lh - 2.f);
                        window.draw(hl);
                        float xPos = listStartX + listAreaWidth - deleteButton.getLocalBounds().width - 25.f;
                        float yPos = listStartY + i * lh + (lh - deleteButton.getCharacterSize()) / 2.f - 8.f;
                        deleteButton.setPosition(xPos, yPos);
                        deleteButtonHotspot = deleteButton.getGlobalBounds();
                        window.draw(deleteButton);
                    }
                    sf::Text entry(saves[i], gameFont, 22);
                    entry.setFillColor(i == selectedIndex ? sf::Color::Black : sf::Color(101,67,33));
                    entry.setPosition(listStartX + 10.f, listStartY + i * lh);
                    window.draw(entry);
                }
            }
            break;
        case GameState::EnterSaveName:
            window.draw(saveDialogSprite); window.draw(inputPrompt); window.draw(inputText);
            // Rysowanie migającego kursora.
            if (fmod(cursorClock.getElapsedTime().asSeconds(), 1.f) < 0.5f) {
                sf::RectangleShape cur({2.f, (float)inputText.getCharacterSize()}); cur.setFillColor({101, 67, 33}); cur.setPosition(inputText.getPosition().x + inputText.getLocalBounds().width + 2.f, inputText.getPosition().y + 8.f); window.draw(cur);
            }
            break;
        case GameState::OptionsMenu:
            window.draw(optsSprite);
            musicSlider.draw(window);
            sfxSlider.draw(window);
            break;
        }

        // Wyświetlenie narysowanej klatki na ekranie.
        window.display();
    }
    return 0;
}
