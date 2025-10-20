#pragma once
#include <SFML/Audio.hpp>
#include <string>

class AudioManager
{
private:
    sf::Music music;

public:
    AudioManager(); // Constructor

    // Chỉ có MỘT phiên bản loadMusic trả về bool
    bool loadMusic(const std::string& filename);

    // Hàm phát nhạc
    void playMusic(float volume = 100.f, bool loop = true);
};
