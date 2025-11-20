#include "AudioManager.h"
#include <algorithm>

AudioManager& AudioManager::getInstance()
{
    static AudioManager instance;
    return instance;
}

bool AudioManager::playMusic(const std::string& filePath, bool loop)
{
    if (!m_music.openFromFile(filePath))
    {
        std::cerr << "[Audio] Failed to load music: " << filePath << std::endl;
        return false;
    }
    m_music.setLoop(loop);
    m_music.setVolume(m_musicVolume);
    m_music.play();
    std::cout << "[Audio] Playing music: " << filePath << std::endl;
    return true;
}

void AudioManager::stopMusic()
{
    m_music.stop();
}

void AudioManager::pauseMusic()
{
    if (m_music.getStatus() == sf::Music::Playing)
    {
        m_music.pause();
    }
}

void AudioManager::resumeMusic()
{
    if (m_music.getStatus() == sf::Music::Paused)
    {
        m_music.play();
        m_music.setVolume(m_musicVolume);
    }
}


void AudioManager::setMusicVolume(float volume)
{
    m_musicVolume = volume;
    m_music.setVolume(volume);
}

float AudioManager::getMusicVolume() const
{
    return m_musicVolume;
}

void AudioManager::loadSound(const std::string& name, const std::string& filePath)
{
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filePath))
    {
        std::cerr << "[Audio] Failed to load sound effect: " << filePath << std::endl;
        return;
    }
    m_soundBuffers[name] = buffer;
    std::cout << "[Audio] Loaded sound effect: " << name << std::endl;
}

void AudioManager::playSound(const std::string& name)
{
    auto it = m_soundBuffers.find(name);
    if (it == m_soundBuffers.end())
    {
        std::cerr << "[Audio] Sound not loaded: " << name << std::endl;
        return;
    }
    
    auto reuse = std::find_if(
        m_activeSounds.begin(), m_activeSounds.end(),
        [](const sf::Sound& sound) { return sound.getStatus() == sf::Sound::Stopped; }
    );
    if (reuse != m_activeSounds.end())
    {
        reuse->setBuffer(it->second);
        reuse->setVolume(m_sfxVolume);
        reuse->play();
    }
    else
    {
        m_activeSounds.emplace_back();
        sf::Sound& activeSound = m_activeSounds.back();
        activeSound.setBuffer(it->second);
        activeSound.setVolume(m_sfxVolume);
        activeSound.play();
    }
}

void AudioManager::setSfxVolume(float volume)
{
    m_sfxVolume = volume;
}

float AudioManager::getSfxVolume() const
{
    return m_sfxVolume;
}
void AudioManager::update()
{
    m_activeSounds.erase(
        std::remove_if(m_activeSounds.begin(), m_activeSounds.end(),
            [](const sf::Sound& s) { return s.getStatus() == sf::Sound::Stopped; }),
        m_activeSounds.end()
    );
}