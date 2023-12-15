#pragma once

#include "qcontainerfwd.h"
#include "qtypes.h"
#include "tile.hpp"
#include "../gui/entity_drawer.hpp"

class Room
{
public:
    Room();
    Room(int, std::pair<int, int>, std::pair<int, int>);
    ~Room();
    qreal getId();
    std::unordered_map<QString, Tile*> get_unused_spawnpoints();
    std::unordered_map<QString, Tile*> get_used_spawnpoints();
    void set_width_and_height();
    void set_end_coords(std::pair<int, int>);
    QVector<EntityDrawer *> get_players_in_room();
    void add_player_to_room(EntityDrawer *);
    std::pair<int, int> get_start_coords();
    std::pair<int, int> get_end_coords();
    int get_width();
    int get_height();
private:
    int id_;
    int width_;
    int height_;
    std::pair<int, int> start_coords_;
    std::pair<int, int> end_coords_;
    std::unordered_map<QString, Tile*> unused_spawnpoints_;
    std::unordered_map<QString, Tile*> used_spawnpoints_;
    QVector<EntityDrawer *> players_in_room_;
};
