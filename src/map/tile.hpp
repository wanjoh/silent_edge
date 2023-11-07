#pragma once

class Tile
{
public:
    enum class TileType
    {
        GROUND,
        WALL
    };

    TileType getTileType();
private:
    TileType type_;
};
