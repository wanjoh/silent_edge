#include "map.hpp"

#include <QFile>
#include <QTextStream>

constexpr static qint32 IMAGE_SIZE = 64;

Map::Map()
{
    // uvek isti
    map_path_ = "../silent-edge/src/map/map_matrix.txt";
    group_ = new QGraphicsItemGroup;
    map_ = initialize_matrix();
}

Map::~Map()
{
    ammo_piles_.clear();
    tile_drawer_map_.clear();
    map_.clear();
    delete group_;
}

std::map<QString, Tile*> Map::initialize_matrix()
{
    std::map<QString, Tile*> *map = new std::map<QString, Tile*>;

    QFile file(map_path_);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        int n, m;
        stream >> n >> m;

        bool is_spawnpoint = false;
        bool is_ammo = false;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                int number;
                stream >> number;

                QString path = "../silent-edge/src/images/";
                Tile::TileType type = Tile::TileType::GROUND;
                switch(number) {
                    // ground
                    case 1:
                        path += "big_ground.png";
                        break;
                    // wall
                    case 2:
                        path += "big_wall.png";
                        type = Tile::TileType::WALL;
                        break;
                    // spawnpoint
                    case 3:
                        path += "spawn_point.png";
                        is_spawnpoint = true;
                        break;
                    // ammo
                    case 4:
                        path += "ammo_bucket.png";
                        type = Tile::TileType::AMMO_PILE;
                        is_ammo = true;
                        break;
                    // default = ground
                    default:
                        path += "big_ground.png";
                        break;
                }
                QString name = QString("%1 %2").arg(j).arg(i);
                Tile *tile = new Tile(name, path, QPair<int, int>(j, i), type);
                TileDrawer *drawer = tile->getDrawer();
                drawer->setPos(j*IMAGE_SIZE, i*IMAGE_SIZE);
                if(is_ammo)
                    ammo_piles_[name] = drawer;
                else if(is_spawnpoint)
                    spawnpoints_[name] = drawer;

                (*map)[name] = tile;
                tile_drawer_map_[name] = drawer;
                group_->addToGroup(drawer);

                is_ammo = is_spawnpoint = false;
            }
        }

        file.close();
    }

    return *map;
}

void Map::remove_tile(QString name)
{
    if(tile_drawer_map_.contains(name))
    {
        if(ammo_piles_.contains(name))
            ammo_piles_.erase(name);

        group_->removeFromGroup(tile_drawer_map_[name]);
        delete tile_drawer_map_[name];
        tile_drawer_map_.erase(name);

        delete map_[name];
        map_.erase(name);
    }
}

void Map::add_ground_tile(QString name, int x, int y)
{
    Tile *tile = new Tile(name, "../silent-edge/src/images/big_ground.png", QPair<int, int>(x, y), Tile::TileType::GROUND);
    TileDrawer *drawer = tile->getDrawer();
    drawer->setPos(x*IMAGE_SIZE, y*IMAGE_SIZE);

    tile_drawer_map_[name] = drawer;
    map_[name] = tile;
    group_->addToGroup(drawer);
}

void Map::restock_ammo_piles()
{
    ammo_piles_.clear();

    for (auto tile : map_) {
        if (tile.second->getTileType() == Tile::TileType::AMMO_PILE) {
            remove_tile(tile.first);

            QPair<int, int> coords = tile.second->get_coords();
            QString name = QString("%1 %2").arg(coords.first).arg(coords.second);
            Tile *tile = new Tile(name, "../silent-edge/src/images/ammo_bucket.png", QPair<int, int>(coords.second, coords.first), Tile::TileType::GROUND);
            TileDrawer *drawer = tile->getDrawer();
            drawer->setPos(coords.second*IMAGE_SIZE, coords.first*IMAGE_SIZE);

            ammo_piles_[name] = drawer;

            tile_drawer_map_[name] = drawer;
            map_[name] = tile;
            group_->addToGroup(drawer);
        }
    }
}

std::map<QString, Tile*> Map::get_matrix()
{
    return map_;
}

QGraphicsItemGroup* Map::get_group()
{
    return group_;
}

std::map<QString, TileDrawer *> Map::get_spawnpoints()
{
    return spawnpoints_;
}

std::map<QString, TileDrawer*> Map::get_ammo_piles()
{
    return ammo_piles_;
}

std::map<QString, TileDrawer*> Map::get_tile_drawer_map()
{
    return tile_drawer_map_;
}
