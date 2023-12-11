#pragma once

#include "room.hpp"
#include "../gui/tile_drawer.hpp"
#include "tile.hpp"

class Map
{
public:
    Map();
    ~Map();
    QGraphicsItemGroup *get_group();
    std::map<QString, Tile*> initialize_matrix();
    std::map<QString, Tile*> get_matrix();
    std::map<QString, TileDrawer*> get_spawnpoints();
    std::map<QString, TileDrawer*> get_ammo_piles();
    std::map<QString, TileDrawer*> get_tile_drawer_map();
    void remove_name_from_ammo_list(QString name);
    void remove_tile(QString name);
    void add_ground_tile(QString name, int x, int y);
    void restock_ammo_piles();
private:
    std::map<QString, Tile*> map_;
    QGraphicsItemGroup* group_;
    QString map_path_;
    std::map<QString, TileDrawer*> tile_drawer_map_;
    std::map<QString, TileDrawer*> spawnpoints_;
    std::map<QString, TileDrawer*> ammo_piles_;
};
