#include "room.hpp"

const int IMAGE_SIZE = 64;

Room::Room() = default;

Room::Room(int id, std::pair<int, int> start_coords, std::pair<int, int> end_coords)
{
    id_ = id;
    start_coords_ = start_coords;
    end_coords_ = end_coords;

    setWidthAndHeight();
}

Room::~Room() = default;

auto Room::getId() const -> qreal
{
    return id_;
}

auto Room::getUnusedSpawnpoints() -> const std::unordered_map<QString, QPair<int, int>> &
{
    return unused_spawnpoints_;
}

auto Room::getUsedSpawnpoints() -> const std::unordered_map<QString, QPair<int, int>> &
{
    return used_spawnpoints_;
}

void Room::setWidthAndHeight()
{
    width_ = end_coords_.first - start_coords_.first;
    height_ = end_coords_.second - start_coords_.second;
}

void Room::setEndCoords(std::pair<int, int> end_coords)
{
    end_coords_ = end_coords;
}

auto Room::getPlayersInRoom() const -> QVector<Player *>
{
    return players_in_room_;
}

void Room::addPlayerToRoom(Player *player)
{
    if(!unused_spawnpoints_.empty())
    {
        std::pair<QString, QPair<int, int>> spawnpoint = *unused_spawnpoints_.begin();
        players_in_room_.append(player);
        player->getDrawer()->setPos(spawnpoint.second.second*IMAGE_SIZE, spawnpoint.second.first*IMAGE_SIZE);
        unused_spawnpoints_.erase(spawnpoint.first);
        used_spawnpoints_[spawnpoint.first] = spawnpoint.second;
    }
}

void Room::removePlayerFromRoom(Player *player)
{
    players_in_room_.removeOne(player);
    for (auto &spawnpoint : used_spawnpoints_) {
        unused_spawnpoints_[spawnpoint.first] = spawnpoint.second;
    }
}

void Room::addSpawnpoint(const QString &name, QPair<int, int> coords)
{
    unused_spawnpoints_[name] = coords;
}

auto Room::getStartCoords() -> std::pair<int, int>
{
    return start_coords_;
}

auto Room::getEndCoords() -> std::pair<int, int>
{
    return end_coords_;
}

auto Room::getWidth() const -> int
{
    return width_;
}

auto Room::getHeight() const -> int
{
    return height_;
}
