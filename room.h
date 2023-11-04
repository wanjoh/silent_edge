#pragma once

#include "turret.h"

class Room
{
public:
    enum class room_type {
        EMPTY,
        COVER
    };

    float get_id();
    std::list<Turret> get_turrets();
    std::string get_room_type();
private:
    int id;
    std::list<Turret> turrets;
    room_type type;
};
