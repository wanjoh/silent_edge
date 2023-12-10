#include "tile.hpp"

Tile::Tile(const QString &path, QObject *parent)
{
    drawer_ = new TileDrawer(path);
}

TileDrawer* Tile::getDrawer()
{
    return drawer_;
}
