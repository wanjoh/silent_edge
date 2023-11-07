#pragma once

#include "qglobal.h"

class Room
{
public:
    enum class RoomType
    {
        EMPTY,
        COVER
    };

    qreal getId();
    QString getRoomType();
private:
    int id_;
    RoomType type_;
};
