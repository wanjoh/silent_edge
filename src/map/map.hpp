#pragma once

#include "room.hpp"
#include "tile.hpp"
#include "../gui/map_drawer.hpp"

class Map
{
public:
    typedef std::unordered_map<int, Tile*> tile_map;
    Map();
    ~Map();
    QGraphicsItemGroup *get_group();
    void removeTile(int id);
    void removeFromActive(int id);
    void addToActive(int id);
    void restockAmmoPiles();
    const Room& getRoomById(int id) const;
    Room *addPlayerToARoom(Player *player);
    const tile_map& getMatrix() const;
    const tile_map& getActiveAmmoBuckets() const;
    const tile_map& getInactiveAmmoBuckets() const;
    const std::unordered_map<int, Room*>& getRooms() const;
    MapDrawer* getDrawer();
    int getN();
    int getM();
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
