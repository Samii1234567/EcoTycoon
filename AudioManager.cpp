#include "AudioManager.h"
#include <iostream>
#include <algorithm> // Potrzebne dla std::clamp

// ===================================================================================
//
//  PLIK IMPLEMENTACYJNY DLA KLASY AUDIOMANAGER
//
//  Zawiera logikę metod zadeklarowanych w AudioManager.h.
//
// ===================================================================================


// --- Funkcja pomocnicza ---

// Wczytuje pojedynczy plik dźwiękowy do bufora i umieszcza go w mapie buforów.
// Upraszcza to kod w metodzie `loadAssets`.
bool loadBuffer(std::map<SoundEffect, sf::SoundBuffer>& buffers, SoundEffect effect, const std::string& path) {
    if (!buffers[effect].loadFromFile(path)) {
        // Jeśli wczytywanie się nie powiedzie, wyświetla błąd w konsoli.
        std::cerr << "Nie mozna wczytac pliku audio: " << path << "\n";
        return false;
    }
    return true;
}

// Implementacja konstruktora.
AudioManager::AudioManager() {}

// Implementacja wczytywania zasobów audio.
bool AudioManager::loadAssets() {
    bool success = true;

    // Wczytywanie muzyki do menu.
    if (!m_menuMusic.openFromFile("audio/main_menu_music.ogg")) {
        std::cerr << "Nie mozna wczytac pliku audio/main_menu_music.ogg\n";
        success = false;
    }
    m_menuMusic.setLoop(true); // Ustawienie zapętlenia dla muzyki.

    // Wczytywanie muzyki do gry.
    if (!m_gameMusic.openFromFile("audio/game_music.ogg")) {
        std::cerr << "Nie mozna wczytac pliku audio/game_music.ogg\n";
        success = false;
    }
    m_gameMusic.setLoop(true);

    // Wczytywanie wszystkich efektów dźwiękowych za pomocą funkcji pomocniczej.
    // Użycie `&=` sprawia, że jeśli jakikolwiek plik się nie wczyta, `success` przyjmie wartość `false`.
    success &= loadBuffer(m_soundBuffers, SoundEffect::ButtonClick, "audio/button_click.wav");
    success &= loadBuffer(m_soundBuffers, SoundEffect::SolarPanel, "audio/solar_panel.wav");
    success &= loadBuffer(m_soundBuffers, SoundEffect::WindTurbine, "audio/wind_turbine.wav");
    success &= loadBuffer(m_soundBuffers, SoundEffect::EnergyStorage, "audio/energy_magazine.wav");
    success &= loadBuffer(m_soundBuffers, SoundEffect::Upgrade, "audio/upgrade_sound.wav");
    success &= loadBuffer(m_soundBuffers, SoundEffect::BuildingPlace, "audio/building_place.wav");
    success &= loadBuffer(m_soundBuffers, SoundEffect::BuildingSell, "audio/building_sell.wav");
    success &= loadBuffer(m_soundBuffers, SoundEffect::CashRegister, "audio/cash_register.wav");
    success &= loadBuffer(m_soundBuffers, SoundEffect::Repair, "audio/repair_sound.wav");
    success &= loadBuffer(m_soundBuffers, SoundEffect::BellNotification, "audio/bell_notification.wav");

    // Po wczytaniu buforów, tworzymy obiekty `sf::Sound` i przypisujemy im odpowiednie bufory.
    for (auto const& [key, val] : m_soundBuffers) {
        m_sounds[key].setBuffer(val);
    }

    return success;
}

// Implementacja odtwarzania efektu dźwiękowego.
void AudioManager::play(SoundEffect effect) {
    // Sprawdzamy, czy dany efekt dźwiękowy istnieje w naszej mapie.
    if (m_sounds.count(effect)) {
        // Jeśli tak, odtwarzamy go.
        m_sounds.at(effect).play();
    }
}

// Implementacja odtwarzania muzyki z menu.
void AudioManager::playMenuMusic() {
    // Zatrzymujemy muzykę z gry, aby uniknąć odtwarzania dwóch ścieżek naraz.
    if (m_gameMusic.getStatus() == sf::Music::Playing) m_gameMusic.stop();
    // Odtwarzamy muzykę z menu, tylko jeśli nie jest już odtwarzana.
    if (m_menuMusic.getStatus() != sf::Music::Playing) m_menuMusic.play();
}

// Implementacja odtwarzania muzyki z gry.
void AudioManager::playGameMusic() {
    // Zatrzymujemy muzykę z menu.
    if (m_menuMusic.getStatus() == sf::Music::Playing) m_menuMusic.stop();
    // Odtwarzamy muzykę z gry.
    if (m_gameMusic.getStatus() != sf::Music::Playing) m_gameMusic.play();
}

// Implementacja zatrzymywania muzyki.
void AudioManager::stopMusic() {
    m_menuMusic.stop();
    m_gameMusic.stop();
}

// Implementacja ustawiania głośności muzyki.
void AudioManager::setMusicVolume(float volume) {
    // Używamy `std::clamp`, aby upewnić się, że głośność jest w prawidłowym zakresie [0, 100].
    float clampedVolume = std::clamp(volume, 0.f, 100.f);
    m_menuMusic.setVolume(clampedVolume);
    m_gameMusic.setVolume(clampedVolume);
}

// Implementacja ustawiania głośności efektów dźwiękowych.
void AudioManager::setSfxVolume(float volume) {
    float clampedVolume = std::clamp(volume, 0.f, 100.f);
    // Ustawiamy głośność dla każdego efektu w pętli.
    for (auto& pair : m_sounds) {
        pair.second.setVolume(clampedVolume);
    }
}
