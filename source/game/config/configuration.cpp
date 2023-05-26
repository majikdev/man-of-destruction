#define TOML_EXCEPTIONS 0
#define TOML_ENABLE_FORMATTERS 0

#include "configuration.h"
#include "toml.hpp"
#include <fstream>

Configuration* pConfig;

// Load the configuration.

Configuration::Configuration(std::string_view path)
    : path(path)
{
    pConfig = this;

    // Load the config from a file or make an empty one.

    toml::parse_result result = toml::parse_file(path);
    toml::table config = result ? result.table() : toml::table();

    // Read the config values or use their defaults.

    cameraShake = config["accessibility"]["camera_shake"].value_or(1.0f);
    pixelScale = config["accessibility"]["pixel_scale"].value_or(4);

    windowWidth = config["graphics"]["window_width"].value_or(960);
    windowHeight = config["graphics"]["window_height"].value_or(720);
    fullscreen = config["graphics"]["fullscreen"].value_or(false);

    masterVolume = config["sound"]["master_volume"].value_or(0.25f);

    saveSlot = config["saves"]["save_slot"].value_or("saves/slot_1.save");

    // Correct the values that are out of range.

    windowWidth = Max(windowWidth, 960);
    windowHeight = Max(windowHeight, 720);

    cameraShake = Max(cameraShake, 0.0f);
    pixelScale = Clamp(pixelScale, 1, Min(windowWidth / 240, windowHeight / 180));

    masterVolume = Max(masterVolume, 0.0f);

    if (result)
    {
        LOG("Loaded configuration from \"" << path << "\".");
    }
    else
    {
        LOG("No configuration found, one will be generated upon exit.");
    }
}

// Save the configuration.

Configuration::~Configuration()
{
    std::ofstream file(path.data());

    // Write the config values to a file.

    file << "[accessibility]\n\n";

    file << "# How much the camera shakes\n# (real number, at least 0)\n";
    file << "camera_shake = " << cameraShake << std::endl;
    file << "# How large each pixel is\n# (integer, at least 1)\n";
    file << "pixel_scale = " << pixelScale << std::endl;

    file << "\n[graphics]\n\n";

    file << "# Width of the window\n# (integer, at least 960)\n";
    file << "window_width = " << windowWidth << std::endl;
    file << "# Height of the window\n# (integer, at least 720)\n";
    file << "window_height = " << windowHeight << std::endl;
    file << "# Should launch in fullscreen?\n# (boolean, true or false)\n";
    file << "fullscreen = " << (fullscreen ? "true" : "false") << std::endl;

    file << "\n[sound]\n\n";

    file << "# How loud all sounds are\n# (real number, at least 0)\n";
    file << "master_volume = " << masterVolume << std::endl;

    file << "\n[saves]\n\n";

    file << "# Path to the current save slot\n# (path, relative to executable)\n";
    file << "save_slot = \"" << saveSlot << "\"" << std::endl;

    LOG("Saved configuration to \"" << path << "\".");
}