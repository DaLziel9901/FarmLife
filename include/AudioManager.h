
#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <memory>
#include <vector>
#include <iostream>

class AudioManager
{
public:
    static AudioManager& getInstance();

    bool playMusic(const std::string& name, bool loop = true);
    void stopMusic();
	void pauseMusic();
	void resumeMusic();
    void setMusicVolume(float volume);
    float getMusicVolume() const;
    void update();

    void playSound(const std::string& name);
    void setSfxVolume(float volume);
    float getSfxVolume() const;

    void loadMusic(const std::string& name, const std::string& filePath);
    void loadSound(const std::string& name, const std::string& filePath);

private:
    AudioManager() = default; 
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    sf::Music m_music; 
    std::map<std::string, sf::SoundBuffer> m_soundBuffers;
    std::vector<sf::Sound> m_activeSounds;

    float m_musicVolume = 50.f;
    float m_sfxVolume = 60.f;

};
