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
    //???? todo: srediti ovo, prenositi preko referenci
    // limun: pokusao
    void initialize_matrix(std::unordered_map<QString, Tile*>& matrix);
    const std::unordered_map<QString, Tile*>& get_matrix() const;
    const std::unordered_map<QString, Tile*>& get_active_ammo_buckets() const;
    const std::unordered_map<QString, Tile*>& get_inactive_ammo_buckets() const;
    const std::unordered_map<int, Room*>& get_rooms() const;
    void remove_tile(const QString &name);
    void add_ground_tile_of_type_ammo(const QString &name, int x, int y);
    void restock_ammo_piles();
    const Room& get_room_by_id(int id) const;
    Room *add_player_to_a_room(EntityDrawer *player);
    void remove_from_active(const QString& name);
private:
    std::unordered_map<QString, Tile*> map_;
    QGraphicsItemGroup* group_;
    QString map_path_;
    std::unordered_map<QString, Tile*> active_ammo_buckets_;
    std::unordered_map<QString, Tile*> inactive_ammo_buckets_;
    std::unordered_map<int, Room*> rooms_;
};
