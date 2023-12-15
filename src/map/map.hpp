#pragma once

#include "room.hpp"
#include "tile.hpp"
#include "../gui/entity_drawer.hpp"

class Map
{
public:
    Map();
    ~Map();
    QGraphicsItemGroup *get_group();
    std::unordered_map<int, Tile*> initialize_matrix();
    std::unordered_map<int, Tile*> get_matrix();
    std::unordered_map<int, Tile*> get_active_ammo_buckets();
    std::unordered_map<int, Tile*> get_inactive_ammo_buckets();
    std::unordered_map<int, Room> get_rooms();
    void remove_name_from_ammo_list(int id);
    void remove_tile(int id);
    void add_ground_tile_of_type_ammo(int id, int x, int y);
    void restock_ammo_piles();
    Room get_room_by_id(int id);
    Room *add_player_to_a_room(EntityDrawer *player);
private:
    std::unordered_map<int, Tile*> map_;
    QGraphicsItemGroup* group_;
    QString map_path_;
    std::unordered_map<int, Tile*> active_ammo_buckets_;
    std::unordered_map<int, Tile*> inactive_ammo_buckets_;
    std::unordered_map<int, Room> rooms_;
};
