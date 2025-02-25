cmake_minimum_required(VERSION 3.5)

project(SilentEdge VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network)

set(PROJECT_SOURCES
        src/main.cpp
        src/gui/mainwindow.cpp
        src/gui/mainwindow.hpp
        src/gui/mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SilentEdge
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/game/game.cpp
        src/game/game.hpp
        src/entity/entity.cpp
        src/entity/entity.hpp
        src/entity/player.cpp
        src/entity/player.hpp
        src/entity/melee_weapon.cpp
        src/entity/melee_weapon.hpp
        src/entity/bullet.cpp
        src/entity/bullet.hpp
        src/entity/turret.cpp
        src/entity/turret.hpp
        src/entity/weapon.cpp
        src/entity/weapon.hpp
        src/entity/ranged_weapon.cpp
        src/entity/ranged_weapon.hpp
        src/map/map.cpp
        src/map/map.hpp
        src/map/room.cpp
        src/map/room.hpp
        src/map/tile.cpp
        src/map/tile.hpp
        src/map/tileset.cpp
        src/map/tileset.hpp
        src/server/server.cpp
        src/server/server.hpp
        src/server/connection.cpp
        src/server/connection.hpp
        src/client/client_manager.hpp
        src/client/client_manager.cpp
        src/gui/entity_drawer.hpp
        src/gui/entity_drawer.cpp
        src/gui/game_window.hpp
        src/gui/game_window.cpp

        src/images/walls/wall.png
        src/images/walls/wall1.png
        src/images/walls/wall2.png
        src/images/walls/wall3.png
        src/images/walls/wall4.png
        src/images/walls/wall5.png
        src/images/walls/wall6.png
        src/images/walls/wall7.png
        src/images/walls/wall8.png
        src/images/walls/wall9.png
        src/images/walls/wall10.png
        src/images/walls/wall11.png
        src/images/walls/wall12.png
        src/images/walls/wall13.png
        src/images/walls/wall14.png
        src/images/walls/wall15.png

        src/images/barrier.png
        src/images/ground.png
        src/images/samurai.png
        src/images/enemy.png
        src/images/spawnpoint.png
        src/images/ammo_bucket.png
        src/images/katana.png
        src/images/katana_overlay.png
        src/images/pistol_overlay.png
        src/images/pistol_overlay_2.png
        src/images/bullet.png
        src/images/reload.png


        src/map/map_matrix.txt
        src/gui/tile_drawer.hpp
        src/gui/tile_drawer.cpp
        src/gui/player_drawer.hpp
        src/gui/player_drawer.cpp
        src/gui/bullet_drawer.hpp
        src/gui/bullet_drawer.cpp
        src/gui/melee_drawer.hpp
        src/gui/melee_drawer.cpp

        src/server/game_logic_handler.hpp
        src/server/game_logic_handler.cpp

        src/server/server_config.hpp
        src/gui/map_drawer.hpp
        src/gui/map_drawer.cpp
        src/gui/lobby.hpp 
        src/gui/lobby.cpp 
        src/gui/lobby.ui
        src/gui/results.hpp
        src/gui/results.cpp
        src/gui/results.ui

    )
    # qt_add_executable(ServerMain src/server/server_main.cpp)
    # # qt_add_executable(ClientMain src/client/client_main.cpp)
    # target_link_libraries(ServerMain INTERFACE SilentEdge)
    else()
if(ANDROID)
    add_library(SilentEdge SHARED
        ${PROJECT_SOURCES}
    )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
else()
    add_executable(SilentEdge
        ${PROJECT_SOURCES}
    )
endif()
endif()

target_link_libraries(SilentEdge PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(SilentEdge PRIVATE Qt${QT_VERSION_MAJOR}::Network)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.SilentEdge)
endif()
set_target_properties(SilentEdge PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS SilentEdge
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SilentEdge)
endif()
