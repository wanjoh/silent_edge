#pragma once

#include "tile.hpp"
#include <list>

class Tileset : public Tile
{
public:
    std::list<Tile> getTiles();
private:
    std::list<Tile> tiles_;
};
