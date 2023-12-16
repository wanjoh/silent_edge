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
    Tile(const QString name = "", const QString &path = "", QPair<int, int> coords = QPair<int, int>(0, 0), TileType type = Tile::TileType::GROUND ,QObject * = nullptr);
    ~Tile();
    TileType getTileType();
    TileDrawer* getDrawer();
    QPair<int, int> get_coords();
    QString get_name();
    void setDrawer(QString path);
private:
    QString name_;
    QPair<int, int> coords_;
    TileType type_;
    TileDrawer *drawer_;
};
