#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "core/minimal.h"
#include <string_view>
#include <string>

extern class Configuration* pConfig;

class Configuration
{
public:
    Configuration(std::string_view path);
    ~Configuration();

private:
    std::string_view path;

public:
    float cameraShake;
    int pixelScale;
    int windowWidth;
    int windowHeight;
    bool fullscreen;
    float masterVolume;
    std::string saveSlot;
};

#endif