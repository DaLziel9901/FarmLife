#include "AudioManager.h"

// --- Singleton Access ---
AudioManager& AudioManager::getInstance()
{
    static AudioManager instance;
    return instance;
}

// --- MUSIC ---
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

void AudioManager::setMusicVolume(float volume)
{
    m_musicVolume = volume;
    m_music.setVolume(volume);
}

float AudioManager::getMusicVolume() const
{
    return m_musicVolume;
}

// --- SOUND EFFECTS ---
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

    sf::Sound sound;
    sound.setBuffer(it->second);
    sound.setVolume(m_sfxVolume);
    sound.play();
    m_activeSounds.push_back(sound);

    // Xóa các sound đã dừng để tránh đầy bộ nhớ
    m_activeSounds.erase(
        std::remove_if(m_activeSounds.begin(), m_activeSounds.end(),
            [](const sf::Sound& s) { return s.getStatus() == sf::Sound::Stopped; }),
        m_activeSounds.end());
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