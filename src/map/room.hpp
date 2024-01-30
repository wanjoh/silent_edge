#pragma once

#include "qcontainerfwd.h"
#include "../entity/player.hpp"

class Room
{
public:
    using spawnpointMap = std::unordered_map<QString, QPair<int, int>>;

    Room();
    Room(int, std::pair<int, int>, std::pair<int, int>);
    ~Room();

    auto getId() const -> qreal;
    auto getUnusedSpawnpoints() -> const spawnpointMap &;
    auto getUsedSpawnpoints() -> const spawnpointMap &;
    void setWidthAndHeight();
    void setEndCoords(std::pair<int, int>);
    auto getPlayersInRoom() const -> QVector<Player *>;
    void addPlayerToRoom(Player *);
    auto getStartCoords() -> std::pair<int, int>;
    auto getEndCoords() -> std::pair<int, int>;
    auto getWidth() const -> int;
    auto getHeight() const -> int;
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
