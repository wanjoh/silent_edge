#pragma once

#include "room.hpp"
#include "tile.hpp"

class Map
{
public:
    Map();
    ~Map();
    QGraphicsItemGroup *get_group();
    std::unordered_map<QString, Tile*> initialize_matrix();
    std::unordered_map<QString, Tile*> get_matrix();
    std::unordered_map<QString, Tile*> get_active_ammo_buckets();
    std::unordered_map<QString, Tile*> get_inactive_ammo_buckets();
    void remove_name_from_ammo_list(QString name);
    void remove_tile(QString name);
    void add_ground_tile_of_type_ammo(QString name, int x, int y);
    void restock_ammo_piles();
private:
    std::unordered_map<QString, Tile*> map_;
    QGraphicsItemGroup* group_;
    QString map_path_;
    std::unordered_map<QString, Tile*> active_ammo_buckets_;
    std::unordered_map<QString, Tile*> inactive_ammo_buckets_;
};
