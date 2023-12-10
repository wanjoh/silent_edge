#pragma once
#include "../gui/tile_drawer.hpp"

#include <QGraphicsPixmapItem>

class Tile : QObject
{
public:
    enum class TileType
    {
        GROUND,
        WALL
    };
    Tile(const QString &path = "", QObject * = nullptr);
    TileType getTileType();
    TileDrawer* getDrawer();
private:
    TileType type_;
    TileDrawer *drawer_;
};
