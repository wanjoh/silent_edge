#pragma once

#include "room.hpp"
#include "../gui/tile_drawer.hpp"

class Map
{
public:
    Map();
    QGraphicsItemGroup *draw_matrix();
    QSharedPointer<QVector<QVector<int>>> make_matrix();
    QSharedPointer<QVector<QVector<int>>> get_matrix();
    std::map<QString, TileDrawer*> get_spawnpoints();
    std::map<QString, TileDrawer*> get_ammo_piles();
    QString get_name(int x, int y);
    void remove_name_from_ammo_list(QString name);
private:
    QSharedPointer<QVector<QVector<int>>> matrix_;
    std::map<QString, TileDrawer*> spawnpoints_;
    std::map<QString, TileDrawer*> ammo_piles_;
    QString map_path_;
};
