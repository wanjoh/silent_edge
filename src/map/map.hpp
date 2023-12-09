#pragma once

#include "room.hpp"
#include "../gui/entity_drawer.hpp"

class Map
{
public:
    Map(QString*);
    QGraphicsItemGroup *draw_matrix(QString* path_to_map);
    QSharedPointer<QVector<QVector<int>>> get_matrix(QString* path_to_map);
};
