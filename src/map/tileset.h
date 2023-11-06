#pragma once

#include "tile.h"
#include <list>

class Tileset : public Tile
{
public:
    std::string get_tiles();
private:
    std::list<Tile> tiles;
};
