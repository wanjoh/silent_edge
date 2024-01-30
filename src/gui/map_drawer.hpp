#pragma once

#include <QGraphicsPixmapItem>
#include "../map/tile.hpp"

class MapDrawer : public QGraphicsPixmapItem
{
public:
    MapDrawer(std::unordered_map<int, Tile*> matrix);
    ~MapDrawer() override;
    void initialize_group(std::unordered_map<int, Tile*>& matrix);
    auto get_group() -> QGraphicsItemGroup *;

    void change_picture(int tile_id, const QString &path);

private:
    QGraphicsItemGroup *group_;
    std::unordered_map<int, Tile*> matrix_;
};
