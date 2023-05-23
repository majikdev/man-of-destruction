#ifndef SOUND_MIXER_H
#define SOUND_MIXER_H

#include "sound.h"
#include <memory>
#include <string_view>
#include <vector>

struct ma_engine;
struct ma_sound;

extern class SoundMixer* pSoundMixer;

class SoundMixer
{
public:
    SoundMixer();
    ~SoundMixer();

    void SetMasterVolume(float volume);
    void PlaySound(const Sound& sound);

    Sound GetSound(std::string_view path);

private:
    std::unique_ptr<ma_engine> pEngine;
    std::vector<std::unique_ptr<ma_sound>> sounds;
    std::vector<std::string_view> files;
};

#endif