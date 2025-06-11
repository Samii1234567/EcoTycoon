#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <SFML/Audio.hpp>
#include <map>
#include <string>

// Enum do łatwej identyfikacji wszystkich efektów dźwiękowych w grze
enum class SoundEffect {
    ButtonClick,
    SolarPanel,
    WindTurbine,
    EnergyStorage,
    Upgrade,
    BuildingPlace,
    BuildingSell,
    CashRegister,
    Repair
};

class AudioManager {
public:
    AudioManager();

    // Wczytuje wszystkie zasoby audio. Zwraca false w przypadku błędu.
    bool loadAssets();

    // Odtwarza dany efekt dźwiękowy
    void play(SoundEffect effect);

    // Kontrola muzyki
    void playMenuMusic();
    void playGameMusic();
    void stopMusic();

    // Ustawia głośność (wartości od 0.0 do 100.0)
    void setMusicVolume(float volume);
    void setSfxVolume(float volume);

private:
    // Obiekty muzyki
    sf::Music m_menuMusic;
    sf::Music m_gameMusic;

    // Kontenery na bufory i dźwięki
    std::map<SoundEffect, sf::SoundBuffer> m_soundBuffers;
    std::map<SoundEffect, sf::Sound> m_sounds;
};

#endif // AUDIOMANAGER_H
