#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <SFML/Audio.hpp>
#include <map>
#include <string>

// ===================================================================================
//
//  PLIK NAGŁÓWKOWY DLA KLASY AUDIOMANAGER
//
//  Definiuje klasę `AudioManager`, która jest centralnym punktem zarządzania
//  wszystkimi dźwiękami w grze. Odpowiada za wczytywanie, odtwarzanie
//  i kontrolowanie głośności zarówno muzyki w tle, jak i krótkich
//  efektów dźwiękowych (SFX).
//
// ===================================================================================


// Typ wyliczeniowy przechowujący wszystkie możliwe efekty dźwiękowe w grze.
// Użycie enuma zamiast stringów zapobiega literówkom i ułatwia zarządzanie.
enum class SoundEffect {
    ButtonClick,
    SolarPanel,
    WindTurbine,
    EnergyStorage,
    Upgrade,
    BuildingPlace,
    BuildingSell,
    CashRegister,
    Repair,
    BellNotification
};

class AudioManager {
public:
    // --- Metody publiczne ---

    // Konstruktor klasy.
    AudioManager();

    // Wczytuje wszystkie zasoby audio (muzykę i efekty) z plików.
    // Zwraca `true`, jeśli wszystkie pliki zostały wczytane pomyślnie.
    bool loadAssets();

    // Odtwarza podany efekt dźwiękowy.
    void play(SoundEffect effect);

    // Odtwarza muzykę z menu głównego, zatrzymując muzykę z gry, jeśli grała.
    void playMenuMusic();

    // Odtwarza muzykę z ekranu rozgrywki, zatrzymując muzykę z menu, jeśli grała.
    void playGameMusic();

    // Zatrzymuje odtwarzanie obu ścieżek muzycznych.
    void stopMusic();

    // Ustawia głośność dla obu ścieżek muzycznych (w zakresie 0-100).
    void setMusicVolume(float volume);

    // Ustawia głośność dla wszystkich efektów dźwiękowych (w zakresie 0-100).
    void setSfxVolume(float volume);

private:
    // --- Prywatne pola klasy ---

    // Obiekty `sf::Music` do strumieniowania długich plików muzycznych.
    sf::Music m_menuMusic;
    sf::Music m_gameMusic;

    // Mapa przechowująca wczytane do pamięci bufory z danymi audio dla krótkich efektów.
    // Kluczem jest typ efektu z enuma `SoundEffect`.
    std::map<SoundEffect, sf::SoundBuffer> m_soundBuffers;

    // Mapa przechowująca gotowe do odtworzenia obiekty `sf::Sound`.
    // Każdy obiekt jest powiązany z odpowiednim buforem.
    std::map<SoundEffect, sf::Sound> m_sounds;
};

#endif // AUDIOMANAGER_H
