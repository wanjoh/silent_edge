#pragma once
#include <iostream>

class Room
{
public:
    enum class room_type {
        EMPTY,
        COVER
    };

    float get_id();
    std::string get_room_type();
private:
    int id;
    room_type type;
};
