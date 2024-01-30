#include "tile.hpp"

Tile::Tile(int id, const QString &path, QPair<int, int> coords, TileType type)
{
    drawer_ = new TileDrawer(path);
    coords_ = coords;
    type_ = type;
    id_ = id;
}

Tile::~Tile()
{

}

int Tile::get_id() const
{
    return id_;
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

void Tile::setDrawer(const QString &path)
{
    drawer_->setImagePath(path);
}
