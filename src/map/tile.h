#pragma once

#include <string>

class Tile {
public:
    enum class tile_type {
        GROUND,
        WALL
    };

    std::string get_tile_type();
private:
    tile_type type;
};
