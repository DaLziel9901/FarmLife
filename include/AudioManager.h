#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <memory>
#include <iostream>

class AudioManager
{
public:
    // --- Singleton Access ---
    static AudioManager& getInstance();

    // --- Music (BGM) ---
    bool playMusic(const std::string& name, bool loop = true);
    void stopMusic();
    void setMusicVolume(float volume);
    float getMusicVolume() const;
    void update();

    // --- Sound Effects (SFX) ---
    void playSound(const std::string& name);
    void setSfxVolume(float volume);
    float getSfxVolume() const;

    // --- Resource Loading ---
    void loadMusic(const std::string& name, const std::string& filePath);
    void loadSound(const std::string& name, const std::string& filePath);

private:
    AudioManager() = default; // Singleton pattern
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    sf::Music m_music; // chỉ 1 bản nhạc nền chạy 1 lúc
    std::map<std::string, sf::SoundBuffer> m_soundBuffers;
    std::vector<sf::Sound> m_activeSounds;

    float m_musicVolume = 50.f;
    float m_sfxVolume = 60.f;
};