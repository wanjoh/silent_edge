#include "map_drawer.hpp"

MapDrawer::MapDrawer(std::unordered_map<int, Tile*> matrix, QGraphicsItem* parent)
{
    group_ = new QGraphicsItemGroup();
    initialize_group(matrix);
}

MapDrawer::~MapDrawer()
{
    delete group_;
}

void MapDrawer::initialize_group(std::unordered_map<int, Tile*>& matrix)
{
    for(auto it = matrix.begin(); it != matrix.end(); it++)
    {
        QPair<int, int> coords = it->second->get_coords();
        TileDrawer *drawer = it->second->getDrawer();
        drawer->setPos(coords.first*IMAGE_SIZE, coords.second*IMAGE_SIZE);

        group_->addToGroup(drawer);
    }
}

QGraphicsItemGroup* MapDrawer::get_group()
{
    return group_;
}

void MapDrawer::change_picture(Tile *tile, const QString& path)
{
    tile->setDrawer(path);
}
