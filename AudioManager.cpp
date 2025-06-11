#include "AudioManager.h"
#include <iostream>
#include <algorithm>

// Funkcja pomocnicza do uproszczenia wczytywania buforów
bool loadBuffer(std::map<SoundEffect, sf::SoundBuffer>& buffers, SoundEffect effect, const std::string& path) {
    if (!buffers[effect].loadFromFile(path)) {
        std::cerr << "Nie mozna wczytac pliku audio: " << path << "\n";
        return false;
    }
    return true;
}

AudioManager::AudioManager() {}

bool AudioManager::loadAssets() {
    bool success = true;

    // Wczytywanie muzyki
    if (!m_menuMusic.openFromFile("audio/main_menu_music.ogg")) {
        std::cerr << "Nie mozna wczytac pliku audio/main_menu_music.ogg\n";
        success = false;
    }
    m_menuMusic.setLoop(true);

    if (!m_gameMusic.openFromFile("audio/game_music.ogg")) {
        std::cerr << "Nie mozna wczytac pliku audio/game_music.ogg\n";
        success = false;
    }
    m_gameMusic.setLoop(true);

    // Wczytywanie efektów dźwiękowych
    success &= loadBuffer(m_soundBuffers, SoundEffect::ButtonClick, "audio/button_click.wav");
    success &= loadBuffer(m_soundBuffers, SoundEffect::SolarPanel, "audio/solar_panel.wav");
    success &= loadBuffer(m_soundBuffers, SoundEffect::WindTurbine, "audio/wind_turbine.wav");
    success &= loadBuffer(m_soundBuffers, SoundEffect::EnergyStorage, "audio/energy_magazine.wav");
    success &= loadBuffer(m_soundBuffers, SoundEffect::Upgrade, "audio/upgrade_sound.wav");
    success &= loadBuffer(m_soundBuffers, SoundEffect::BuildingPlace, "audio/building_place.wav");
    success &= loadBuffer(m_soundBuffers, SoundEffect::BuildingSell, "audio/building_sell.wav");
    success &= loadBuffer(m_soundBuffers, SoundEffect::CashRegister, "audio/cash_register.wav");
    success &= loadBuffer(m_soundBuffers, SoundEffect::Repair, "audio/repair_sound.wav");

    // Przypisanie buforów do obiektów dźwiękowych
    for (auto const& [key, val] : m_soundBuffers) {
        m_sounds[key].setBuffer(val);
    }

    return success;
}

void AudioManager::play(SoundEffect effect) {
    if (m_sounds.count(effect)) {
        m_sounds.at(effect).play();
    }
}

void AudioManager::playMenuMusic() {
    if (m_gameMusic.getStatus() == sf::Music::Playing) m_gameMusic.stop();
    if (m_menuMusic.getStatus() != sf::Music::Playing) m_menuMusic.play();
}

void AudioManager::playGameMusic() {
    if (m_menuMusic.getStatus() == sf::Music::Playing) m_menuMusic.stop();
    if (m_gameMusic.getStatus() != sf::Music::Playing) m_gameMusic.play();
}

void AudioManager::stopMusic() {
    m_menuMusic.stop();
    m_gameMusic.stop();
}

void AudioManager::setMusicVolume(float volume) {
    float clampedVolume = std::clamp(volume, 0.f, 100.f);
    m_menuMusic.setVolume(clampedVolume);
    m_gameMusic.setVolume(clampedVolume);
}

void AudioManager::setSfxVolume(float volume) {
    float clampedVolume = std::clamp(volume, 0.f, 100.f);
    for (auto& pair : m_sounds) {
        pair.second.setVolume(clampedVolume);
    }
}
