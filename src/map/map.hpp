#pragma once

#include "room.hpp"
#include "tile.hpp"
#include "../gui/map_drawer.hpp"

class Map
{
public:
    Map();
    ~Map();
    QGraphicsItemGroup *get_group();
    //???? todo: srediti ovo, prenositi preko referenci
    // limun: pokusao, verovatno uspeo
    void initialize_matrix();
    void remove_tile(int id);
    void remove_from_active(int id);
    void add_to_active(int id);
    void restock_ammo_piles();
    const Room& get_room_by_id(int id) const;
    Room *add_player_to_a_room(EntityDrawer *player);
    const std::unordered_map<int, Tile*>& get_matrix() const;
    const std::unordered_map<int, Tile*>& get_active_ammo_buckets() const;
    const std::unordered_map<int, Tile*>& get_inactive_ammo_buckets() const;
    const std::unordered_map<int, Room*>& get_rooms() const;
    MapDrawer* get_drawer();
private:
    QString map_path_;
    std::unordered_map<int, Tile*> matrix_;
    std::unordered_map<int, Tile*> active_ammo_buckets_;
    std::unordered_map<int, Tile*> inactive_ammo_buckets_;
    std::unordered_map<int, Room*> rooms_;
    MapDrawer* drawer_;
};
