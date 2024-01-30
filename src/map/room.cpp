#include "room.hpp"

const int IMAGE_SIZE = 64;

Room::Room()
{

}

Room::Room(int id, std::pair<int, int> start_coords, std::pair<int, int> end_coords)
{
    id_ = id;
    start_coords_ = start_coords;
    end_coords_ = end_coords;

    setWidthAndHeight();
}

Room::~Room()
{

}

qreal Room::getId() const
{
    return id_;
}

const std::unordered_map<QString ,QPair<int, int>>& Room::getUnusedSpawnpoints()
{
    return unused_spawnpoints_;
}

const std::unordered_map<QString, QPair<int, int>>& Room::getUsedSpawnpoints()
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

QVector<Player *> Room::getPlayersInRoom() const
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

std::pair<int, int> Room::getStartCoords()
{
    return start_coords_;
}

std::pair<int, int> Room::getEndCoords()
{
    return end_coords_;
}

int Room::getWidth() const
{
    return width_;
}

int Room::getHeight() const
{
    return height_;
}
