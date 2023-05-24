cmake_minimum_required(VERSION 3.23)
project(ManOfDestruction)

set(CMAKE_CXX_STANDARD 17)

# Windows build setup.

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(ICON_RESOURCE "icon.rc")

# Add include directories.

include_directories(source
    depend/glad/include
    depend/glfw/include
    depend/miniaudio
    depend/toml++)

# Add library directories.

link_directories(depend/glfw/lib)

# Create and link executable.

add_executable(ManOfDestruction
    depend/glad/src/gl.c
    source/core/audio/sound.h
    source/core/audio/sound_mixer.cpp
    source/core/audio/sound_mixer.h
    source/core/input/action.h
    source/core/input/button.h
    source/core/input/controller.cpp
    source/core/input/controller.h
    source/core/maths/maths.cpp
    source/core/maths/maths.h
    source/core/video/renderer.cpp
    source/core/video/renderer.h
    source/core/video/sprite.h
    source/core/video/sprite_sheet.h
    source/core/video/window.cpp
    source/core/video/window.h
    source/core/logging.h
    source/core/minimal.h
    source/game/camera/camera.cpp
    source/game/camera/camera.h
    source/game/config/configuration.cpp
    source/game/config/configuration.h
    source/game/entity/dynamite.cpp
    source/game/entity/dynamite.h
    source/game/entity/dynamite_pickup.cpp
    source/game/entity/dynamite_pickup.h
    source/game/entity/entity.cpp
    source/game/entity/entity.h
    source/game/entity/player.cpp
    source/game/entity/player.h
    source/game/entity/splinter.cpp
    source/game/entity/splinter.h
    source/game/level/level.cpp
    source/game/level/level.h
    source/game/level/level_list.h
    source/game/menu/level_complete_menu.cpp
    source/game/menu/level_complete_menu.h
    source/game/menu/level_select_menu.cpp
    source/game/menu/level_select_menu.h
    source/game/menu/main_menu.cpp
    source/game/menu/main_menu.h
    source/game/menu/menu.cpp
    source/game/menu/menu.h
    source/game/menu/pause_menu.cpp
    source/game/menu/pause_menu.h
    source/game/save/save_slot.cpp
    source/game/save/save_slot.h
    source/main.cpp
    ${ICON_RESOURCE})

target_link_libraries(ManOfDestruction glfw3)
set_target_properties(ManOfDestruction PROPERTIES WIN32_EXECUTABLE $<CONFIG:Release>)