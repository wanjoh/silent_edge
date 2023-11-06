qt_add_library(
    SilentEdgeMap
    STATIC
    map.cpp
    map.h
    room.cpp
    room.h
    tile.cpp
    tile.h
    tileset.cpp
    tileset.h
)

target_link_libraries(SilentEdgeMap PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(SilentEdgeMap INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})