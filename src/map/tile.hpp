#pragma once
#include "../gui/tile_drawer.hpp"

#include <QGraphicsPixmapItem>

const int IMAGE_SIZE = 64;

class Tile : QObject
{
public:
    enum class TileType
    {
        GROUND,
        WALL,
        AMMO_PILE
    };
    Tile(int id = 0, const QString &path = "", QPair<int, int> coords = QPair<int, int>(0, 0), TileType type = Tile::TileType::GROUND);
    ~Tile();
    TileType getTileType();
    TileDrawer* getDrawer();
    QPair<int, int> get_coords();
    int get_id() const;
    void setDrawer(const QString &path);
private:
    int id_;
    QPair<int, int> coords_;
    TileType type_;
    TileDrawer *drawer_;
};
