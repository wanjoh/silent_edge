#include "tile.hpp"

Tile::Tile(const QString name, const QString &path, QPair<int, int> coords, TileType type, QObject *parent)
{
    drawer_ = new TileDrawer(path);
    coords_ = coords;
    type_ = type;
    name_ = name;
}

Tile::~Tile()
{

}

QString Tile::get_name()
{
    return name_;
}

QPair<int, int> Tile::get_coords()
{
    return coords_;
}

Tile::TileType Tile::getTileType()
{
    return type_;
}

TileDrawer* Tile::getDrawer()
{
    return drawer_;
}
