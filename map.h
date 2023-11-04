#pragma once

#include "game.h"
#include "room.h"
#include <list>

class Map : public Game
{
public:
    int get_number_of_rooms();
    std::list<Room> get_rooms();
private:
    int number_of_rooms;
    std::list<int> rooms;
};
