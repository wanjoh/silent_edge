#include "tile.hpp"

Tile::Tile(int id, const QString &path, QPair<int, int> coords, TileType type)
{
    drawer_ = new TileDrawer(path);
    coords_ = coords;
    type_ = type;
    id_ = id;
}

Tile::~Tile() = default;

auto Tile::get_id() const -> int
{
    return id_;
}

auto Tile::get_coords() -> QPair<int, int>
{
    return coords_;
}

auto Tile::getTileType() -> Tile::TileType
{
    return type_;
}

auto Tile::getDrawer() -> TileDrawer *
{
    return drawer_;
}

void Tile::setDrawer(const QString &path)
{
    drawer_->setImagePath(path);
}
