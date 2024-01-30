#pragma once

#include "tile.hpp"
#include <list>

class Tileset : public Tile
{
public:
    auto getTiles() -> std::list<Tile>;

private:
    std::list<Tile> tiles_;
};
