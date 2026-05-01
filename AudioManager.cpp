// AudioManager.cpp - Audio management implementation
#include "AudioManager.h"
#include <iostream>
AudioManager::AudioManager() : volume(50.0f), muted(false), musicLoaded(false) {
}
AudioManager::~AudioManager() {
    if (musicLoaded) {
        bgMusic.stop();
    }
}
void AudioManager::setVolume(float vol) {
    if (vol < 0.0f) vol = 0.0f;
    if (vol > 100.0f) vol = 100.0f;
    volume = vol;
    if (!muted && musicLoaded) {
        bgMusic.setVolume(volume);
    }
}
float AudioManager::getVolume() const {
    return volume;
}
void AudioManager::toggleMute() {
    muted = !muted;
    if (musicLoaded) {
        if (muted) {
            bgMusic.setVolume(0.0f);
        } else {
            bgMusic.setVolume(volume);
        }
    }
}
bool AudioManager::isMuted() const {
    return muted;
}
bool AudioManager::playMusic(const char* filename) {
    if (bgMusic.openFromFile(filename)) {
        musicLoaded = true;
        bgMusic.setLooping(true);
        if (muted) {
            bgMusic.setVolume(0.0f);
        } else {
            bgMusic.setVolume(volume);
        }
        bgMusic.play();
        return true;
    }
    std::cout << "Warning: Could not load music file: " << filename << std::endl;
    musicLoaded = false;
    return false;
}
void AudioManager::stopMusic() {
    if (musicLoaded) {
        bgMusic.stop();
    }
}