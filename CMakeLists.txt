cmake_minimum_required(VERSION 3.28.3)
project(cosmic-runner)

set(SOURCE_FILES
src/android_leaderboard.cpp
src/android_achievements.cpp
src/background.cpp
src/button_events_game.cpp
src/console_commands_defs.cpp
src/data_manager_defs.cpp
src/debris.cpp
src/debris_type.cpp
src/dev_info.cpp
src/directories_defs.cpp
src/dodging.cpp
src/effect.cpp
src/engine_glue.cpp
src/explosion.cpp
src/game.cpp
src/high_score_table.cpp
src/high_score.cpp
src/game_constants.cpp
src/game_data.cpp
src/game_input_defs.cpp
src/game_manager_defs.cpp
src/game_options.cpp
src/hud.cpp
src/item.cpp
src/item_type.cpp
src/main.cpp
src/minimap.cpp
src/network_game.cpp
src/planet_type.cpp
src/planet.cpp
src/ship.cpp
src/ship_type.cpp
src/shot.cpp
src/shot_type.cpp
src/special_info.cpp
src/title.cpp
src/title_ship.cpp
src/title_sun.cpp
src/title_tree.cpp
src/upgrade.cpp
src/version.cpp
src/window_close_function.cpp
src/window_scrolling_buttons.cpp
game.rc
)

########################################################################################################################
# Release-Linux-x86_64
########################################################################################################################

add_executable(Release-Linux-x86_64 ${SOURCE_FILES})

set_target_properties(Release-Linux-x86_64 PROPERTIES
OUTPUT_NAME Cosmic-Runner-Linux-x86_64
COMPILE_FLAGS "-fexpensive-optimizations -O2 -std=c++11 -Wall -Wextra -m64 -DGAME_OS_LINUX"
)

target_include_directories(Release-Linux-x86_64 PRIVATE
/usr/include/
/usr/include/x86_64-linux-gnu/
/home/tails/build-server/steamworks-sdk/public/steam/
/home/tails/build-server/linux-x86_64/SDL2/include/
/home/tails/build-server/linux-x86_64/SDL2_image/
/home/tails/build-server/linux-x86_64/SDL2_mixer/include/
/home/tails/build-server/linux-x86_64/boost/
/home/tails/build-server/linux-x86_64/raknet/
/home/tails/build-server/linux-x86_64/zlib/
/home/tails/build-server/cheese-engine/src/
)

target_link_libraries(Release-Linux-x86_64
/home/tails/build-server/cheese-engine/libCheese-Engine-Linux-x86_64.a
/home/tails/build-server/linux-x86_64/zlib/contrib/minizip/.libs/libminizip.a
/home/tails/build-server/linux-x86_64/SDL2_image/.libs/libSDL2_image.a
/home/tails/build-server/linux-x86_64/zlib/libz.a
/home/tails/build-server/linux-x86_64/SDL2_mixer/build/.libs/libSDL2_mixer.a
/home/tails/build-server/linux-x86_64/SDL2/build/.libs/libSDL2.a
/home/tails/build-server/linux-x86_64/SDL2/build/.libs/libSDL2main.a
/usr/lib/x86_64-linux-gnu/libGL.so
/usr/lib/x86_64-linux-gnu/libGLU.so
/usr/lib/x86_64-linux-gnu/libpthread.a
/usr/lib/x86_64-linux-gnu/libdl.a
/home/tails/build-server/linux-x86_64/boost/stage/lib/libboost_system.a
/home/tails/build-server/linux-x86_64/boost/stage/lib/libboost_filesystem.a
/home/tails/build-server/linux-x86_64/raknet/raknet/Lib/LibStatic/Lib/libRakNetLibStatic.a
/home/tails/build-server/steamworks-sdk/redistributable_bin/linux64/libsteam_api.so
"-s -m64"
)

########################################################################################################################
# Debug-Linux-x86_64
########################################################################################################################

add_executable(Debug-Linux-x86_64 ${SOURCE_FILES})

set_target_properties(Debug-Linux-x86_64 PROPERTIES
OUTPUT_NAME Cosmic-Runner-Linux-Debug-x86_64
COMPILE_FLAGS "-std=c++11 -Wall -Wextra -g -m64 -DGAME_OS_LINUX"
)

target_include_directories(Debug-Linux-x86_64 PRIVATE
/usr/include/
/usr/include/x86_64-linux-gnu/
/home/tails/build-server/steamworks-sdk/public/steam/
/home/tails/build-server/linux-x86_64/SDL2/include/
/home/tails/build-server/linux-x86_64/SDL2_image/
/home/tails/build-server/linux-x86_64/SDL2_mixer/include/
/home/tails/build-server/linux-x86_64/boost/
/home/tails/build-server/linux-x86_64/raknet/
/home/tails/build-server/linux-x86_64/zlib/
/home/tails/build-server/cheese-engine/src/
)

target_link_libraries(Debug-Linux-x86_64
/home/tails/build-server/cheese-engine/libCheese-Engine-Linux-Debug-x86_64.a
/home/tails/build-server/linux-x86_64/zlib/contrib/minizip/.libs/libminizip.a
/home/tails/build-server/linux-x86_64/SDL2_image/.libs/libSDL2_image.a
/home/tails/build-server/linux-x86_64/zlib/libz.a
/home/tails/build-server/linux-x86_64/SDL2_mixer/build/.libs/libSDL2_mixer.a
/home/tails/build-server/linux-x86_64/SDL2/build/.libs/libSDL2.a
/home/tails/build-server/linux-x86_64/SDL2/build/.libs/libSDL2main.a
/usr/lib/x86_64-linux-gnu/libGL.so
/usr/lib/x86_64-linux-gnu/libGLU.so
/usr/lib/x86_64-linux-gnu/libpthread.a
/usr/lib/x86_64-linux-gnu/libdl.a
/home/tails/build-server/linux-x86_64/boost/stage/lib/libboost_system.a
/home/tails/build-server/linux-x86_64/boost/stage/lib/libboost_filesystem.a
/home/tails/build-server/linux-x86_64/raknet/raknet/Lib/LibStatic/Lib/libRakNetLibStatic.a
/home/tails/build-server/steamworks-sdk/redistributable_bin/linux64/libsteam_api.so
"-m64"
)

########################################################################################################################
# Release-Windows-x86_64
########################################################################################################################

if(NOT CMAKE_EXPORT_COMPILE_COMMANDS)

add_executable(Release-Windows-x86_64 ${SOURCE_FILES})

set_target_properties(Release-Windows-x86_64 PROPERTIES
OUTPUT_NAME Cosmic-Runner-Windows-x86_64
SUFFIX .exe
COMPILE_FLAGS "-fexpensive-optimizations -O2 -std=c++11 -Wall -Wextra -m64 -DGAME_OS_WINDOWS"
)

target_include_directories(Release-Windows-x86_64 PRIVATE
/usr/x86_64-w64-mingw32/include
/home/tails/build-server/steamworks-sdk/public/steam/
/home/tails/build-server/windows-x86_64/SDL2/include/
/home/tails/build-server/windows-x86_64/SDL2_image/
/home/tails/build-server/windows-x86_64/SDL2_mixer/include/
/home/tails/build-server/windows-x86_64/boost/
/home/tails/build-server/windows-x86_64/raknet/
/home/tails/build-server/windows-x86_64/zlib/
/home/tails/build-server/cheese-engine/src/
)

target_link_libraries(Release-Windows-x86_64
/usr/x86_64-w64-mingw32/lib/libmingw32.a
/home/tails/build-server/cheese-engine/libCheese-Engine-Windows-x86_64.a
/home/tails/build-server/windows-x86_64/zlib/contrib/minizip/.libs/libminizip.a
/home/tails/build-server/windows-x86_64/SDL2_image/.libs/libSDL2_image.a
/home/tails/build-server/windows-x86_64/zlib/libz.a
/home/tails/build-server/windows-x86_64/SDL2_mixer/build/.libs/libSDL2_mixer.a
/home/tails/build-server/windows-x86_64/SDL2/build/.libs/libSDL2.a
/home/tails/build-server/windows-x86_64/SDL2/build/.libs/libSDL2main.a
/home/tails/build-server/windows-x86_64/boost/stage/lib/libboost_system.a
/home/tails/build-server/windows-x86_64/boost/stage/lib/libboost_filesystem.a
/home/tails/build-server/windows-x86_64/raknet/raknet/Lib/DLL/Lib/libRakNet.a
/home/tails/build-server/steamworks-sdk/redistributable_bin/win64/steam_api64.lib
/usr/x86_64-w64-mingw32/lib/libhid.a
/usr/x86_64-w64-mingw32/lib/libsetupapi.a
/usr/x86_64-w64-mingw32/lib/libws2_32.a
/usr/x86_64-w64-mingw32/lib/libwinmm.a
/usr/x86_64-w64-mingw32/lib/libole32.a
/usr/x86_64-w64-mingw32/lib/libimm32.a
/usr/x86_64-w64-mingw32/lib/libgdi32.a
/usr/x86_64-w64-mingw32/lib/liboleaut32.a
/usr/x86_64-w64-mingw32/lib/libversion.a
"-s -m64 -Wl,--stack=67108864 -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic"
)

endif()

########################################################################################################################
# Release-macOS-arm64
########################################################################################################################

if(NOT CMAKE_EXPORT_COMPILE_COMMANDS)

add_executable(Release-macOS-arm64 ${SOURCE_FILES})

set_target_properties(Release-macOS-arm64 PROPERTIES
OUTPUT_NAME Cosmic-Runner-macOS-arm64
COMPILE_FLAGS "-O2 -std=c++11 -Wall -Wextra -arch arm64 -stdlib=libc++ -mmacosx-version-min=15.0.0 -DGAME_OS_MACOS"
)

target_include_directories(Release-macOS-arm64 PRIVATE
/home/tails/build-server/macos-arm64/osxcross/target/SDK/MacOSX.sdk/usr/include/
/home/tails/build-server/steamworks-sdk/public/steam/
/home/tails/build-server/macos-arm64/SDL2/include/
/home/tails/build-server/macos-arm64/SDL2_image/
/home/tails/build-server/macos-arm64/SDL2_mixer/include/
/home/tails/build-server/macos-arm64/boost/
/home/tails/build-server/macos-arm64/raknet/
/home/tails/build-server/macos-arm64/zlib/
/home/tails/build-server/cheese-engine/src/
)

target_link_libraries(Release-macOS-arm64
/home/tails/build-server/cheese-engine/libCheese-Engine-macOS-arm64.a
/home/tails/build-server/macos-arm64/zlib/contrib/minizip/.libs/libminizip.a
/home/tails/build-server/macos-arm64/SDL2_image/.libs/libSDL2_image.a
/home/tails/build-server/macos-arm64/zlib/libz.a
/home/tails/build-server/macos-arm64/SDL2_mixer/build/.libs/libSDL2_mixer.a
/home/tails/build-server/macos-arm64/SDL2/build/.libs/libSDL2.a
/home/tails/build-server/macos-arm64/SDL2/build/.libs/libSDL2main.a
/home/tails/build-server/macos-arm64/boost/stage/lib/libboost_system.a
/home/tails/build-server/macos-arm64/boost/stage/lib/libboost_filesystem.a
/home/tails/build-server/macos-arm64/raknet/raknet/Lib/LibStatic/Lib/libRakNetLibStatic.a
/home/tails/build-server/steamworks-sdk/redistributable_bin/osx/libsteam_api.dylib
"-L/home/tails/build-server/macos-arm64/osxcross/target/SDK/MacOSX.sdk/usr/lib -arch arm64 -stdlib=libc++ -mmacosx-version-min=15.0.0 -liconv -framework CoreFoundation -framework ApplicationServices -framework IOKit -framework AppKit -framework Carbon -framework ForceFeedback -framework CoreAudio -framework AudioUnit -framework AudioToolbox -framework OpenGL -framework CoreVideo -framework CoreHaptics -framework GameController"
)

endif()

########################################################################################################################
# Android-NoBuild
########################################################################################################################

if(NOT CMAKE_EXPORT_COMPILE_COMMANDS)

add_executable(Android-NoBuild ${SOURCE_FILES})

set_target_properties(Android-NoBuild PROPERTIES
OUTPUT_NAME Cosmic-Runner-Android
COMPILE_FLAGS "-fexpensive-optimizations -O2 -std=c++11 -Wall -Wextra -m64 -DGAME_OS_ANDROID"
)

target_include_directories(Android-NoBuild PRIVATE
/usr/include/
/usr/include/x86_64-linux-gnu/
/home/tails/build-server/android/SDL2/include/
/home/tails/build-server/android/SDL2_image/
/home/tails/build-server/android/SDL2_mixer/include/
/home/tails/build-server/linux-x86_64/boost/
/home/tails/build-server/android/raknet/
)

target_link_libraries(Android-NoBuild
"-s -m64"
)

endif()
