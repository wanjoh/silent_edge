#pragma once

#include "qcontainerfwd.h"
#include "../entity/player.hpp"

class Room
{
public:
    typedef std::unordered_map<QString, QPair<int, int>> spawnpointMap;

    Room();
    Room(int, std::pair<int, int>, std::pair<int, int>);
    ~Room();

    qreal getId();
    const spawnpointMap& getUnusedSpawnpoints();
    const spawnpointMap& getUsedSpawnpoints();
    void setWidthAndHeight();
    void setEndCoords(std::pair<int, int>);
    QVector<Player *> getPlayersInRoom();
    void addPlayerToRoom(Player *);
    std::pair<int, int> getStartCoords();
    std::pair<int, int> getEndCoords();
    int getWidth();
    int getHeight();
    void addSpawnpoint(const QString &name, QPair<int, int> coords);
    void removePlayerFromRoom(Player *player);
    QVector<Player *> players_in_room_;
private:
    int id_;
    int width_;
    int height_;
    std::pair<int, int> start_coords_;
    std::pair<int, int> end_coords_;
    spawnpointMap unused_spawnpoints_;
    spawnpointMap used_spawnpoints_;
//    QVector<Player *> players_in_room_;
};
