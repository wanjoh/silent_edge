#pragma once

#include "room.hpp"
#include "tile.hpp"
#include "../gui/map_drawer.hpp"

class Map
{
public:
    using tile_map = std::unordered_map<int, Tile *>;
    Map();
    ~Map();
    auto get_group() -> QGraphicsItemGroup *;
    void removeTile(int id);
    void removeFromActive(int id);
    void addToActive(int id);
    void restockAmmoPiles();
    auto getRoomById(int id) const -> const Room &;
    auto findRoomForPlayer(Player &) -> Room *;
    auto getMatrix() const -> const tile_map &;
    auto getActiveAmmoBuckets() const -> const tile_map &;
    auto getInactiveAmmoBuckets() const -> const tile_map &;
    auto getRooms() const -> const std::unordered_map<int, Room *> &;
    auto getDrawer() -> MapDrawer *;
    auto getN() const -> int;
    auto getM() const -> int;

private:
    void initializeMatrix();
    QString map_path_;
    tile_map matrix_;
    tile_map active_ammo_buckets_;
    tile_map inactive_ammo_buckets_;
    std::unordered_map<int, Room*> rooms_;
    MapDrawer* drawer_;
    int n_;
    int m_;
};
