#pragma once
// AudioManager.h - Manages background music and volume (ENCAPSULATION)
#include <SFML/Audio/Music.hpp>
class AudioManager {
private:
    // ENCAPSULATION: all data is private, accessed only through public methods
    sf::Music bgMusic;
    float volume;
    bool muted;
    bool musicLoaded;
public:
    AudioManager();
    ~AudioManager();
    // Public interface for volume control (ENCAPSULATION)
    void setVolume(float vol);
    float getVolume() const;
    void toggleMute();
    bool isMuted() const;
    // Music control
    bool playMusic(const char* filename);
    void stopMusic();
};