#pragma once

#include <QGraphicsPixmapItem>
#include "../map/tile.hpp"

class MapDrawer : public QGraphicsPixmapItem
{
public:
    MapDrawer(std::unordered_map<int, Tile*> matrix, QGraphicsItem * = nullptr);
    ~MapDrawer();
    void initialize_group(std::unordered_map<int, Tile*>& matrix);
    QGraphicsItemGroup *get_group();
public slots:
    void change_picture(Tile *tile, const QString& path);
private:
    QGraphicsItemGroup *group_;
};
