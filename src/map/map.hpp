#pragma once

#include "room.hpp"
#include <list>

class Map
{
public:
    int getNumberOfRooms();
    std::list<Room> getRooms();
private:
    qint32 number_of_rooms_;
    std::list<qint32> rooms_;
};
