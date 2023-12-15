#include "room.hpp"

Room::Room()
{

}

Room::Room(int id, std::pair<int, int> start_coords, std::pair<int, int> end_coords)
{
    id_ = id;
    start_coords_ = start_coords;
    end_coords_ = end_coords;

    set_width_and_height();
}

Room::~Room()
{

}

qreal Room::getId()
{
    return id_;
}

std::unordered_map<QString, Tile*> Room::get_unused_spawnpoints()
{
    return unused_spawnpoints_;
}

std::unordered_map<QString, Tile*> Room::get_used_spawnpoints()
{
    return used_spawnpoints_;
}

void Room::set_width_and_height()
{
    width_ = end_coords_.first - start_coords_.first;
    height_ = end_coords_.second - start_coords_.second;
}

void Room::set_end_coords(std::pair<int, int> end_coords)
{
    end_coords_ = end_coords;
}

QVector<EntityDrawer *> Room::get_players_in_room()
{
    return players_in_room_;
}

void Room::add_player_to_room(EntityDrawer *player)
{
    players_in_room_.append(player);
}

std::pair<int, int> Room::get_start_coords()
{
    return start_coords_;
}

std::pair<int, int> Room::get_end_coords()
{
    return end_coords_;
}

int Room::get_width()
{
    return width_;
}

int Room::get_height()
{
    return height_;
}
