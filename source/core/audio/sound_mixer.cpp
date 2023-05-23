#define MINIAUDIO_IMPLEMENTATION
#define MA_NO_MP3
#define MA_NO_FLAC
#define MA_NO_GENERATION
#define MA_NO_NULL

#include "sound_mixer.h"
#include "core/logging.h"
#include "miniaudio.h"

#undef PlaySound

SoundMixer* pSoundMixer;

// Load sound data from a file.

static void LoadSoundFile(std::string_view path, std::unique_ptr<ma_engine>& pEngine, std::unique_ptr<ma_sound>& outSound)
{
    constexpr ma_uint32 flags = MA_SOUND_FLAG_NO_PITCH | MA_SOUND_FLAG_NO_SPATIALIZATION;
    ma_result result = ma_sound_init_from_file(pEngine.get(), path.data(), flags, nullptr, nullptr, outSound.get());

    if (result == MA_SUCCESS)
    {
        LOG("Loaded sound from \"" << path << "\".");
    }
    else
    {
        ERR("Failed to load sound from \"" << path << "\".");
    }
}

// Initialise the sound mixer.

SoundMixer::SoundMixer()
{
    pSoundMixer = this;

    pEngine = std::make_unique<ma_engine>();
    ma_engine_init(nullptr, pEngine.get());

    LOG("Initialised the Sound Mixer.");
}

// Terminate the sound mixer.

SoundMixer::~SoundMixer()
{
    for (std::unique_ptr<ma_sound>& pSound : sounds)
    {
        ma_sound_uninit(pSound.get());
    }

    ma_engine_uninit(pEngine.get());
}

// Set the master volume multiplier.

void SoundMixer::SetMasterVolume(float volume)
{
    ma_engine_set_volume(pEngine.get(), volume);
}

// Play a specified sound.

void SoundMixer::PlaySound(const Sound& sound)
{
    ma_engine_play_sound(pEngine.get(), files[sound.identifier].data(), nullptr);
}

// Get a sound from file path.

Sound SoundMixer::GetSound(std::string_view path)
{
    auto location = std::find(files.begin(), files.end(), path);

    // If the sound is already loaded, return it.

    if (location != files.end())
    {
        return {(int) (location - files.begin())};
    }

    // Otherwise, load the sound from a file.

    files.emplace_back(path);
    sounds.push_back(std::make_unique<ma_sound>());

    LoadSoundFile(path, pEngine, sounds.back());

    return {(int) files.size() - 1};
}