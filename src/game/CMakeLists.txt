qt_add_library(
    SilentEdgeGame
    game.cpp
    game.h
)

target_link_libraries(SilentEdgeGame PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(SilentEdgeGame INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})