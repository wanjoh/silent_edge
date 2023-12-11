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
    active_ammo_buckets_.clear();
    map_.clear();
    delete group_;
}

std::unordered_map<QString, Tile*> Map::initialize_matrix()
{
    std::unordered_map<QString, Tile*> *map = new std::unordered_map<QString, Tile*>;

    QFile file(map_path_);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        int n, m;
        stream >> n >> m;

        float is_ammo = false;
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

                (*map)[name] = tile;
                group_->addToGroup(drawer);

                if(is_ammo)
                    active_ammo_buckets_[name] = tile;

                is_ammo = false;
            }
        }

        file.close();
    }

    return *map;
}

void Map::remove_tile(QString name)
{
    if(active_ammo_buckets_.contains(name))
        active_ammo_buckets_.erase(name);
}

void Map::add_ground_tile_of_type_ammo(QString name, int x, int y)
{
    // tipa AMMO, zato što se tu stvara AMMO
    Tile *tile = new Tile(name, "../silent-edge/src/images/big_ground.png", QPair<int, int>(x, y), Tile::TileType::AMMO_PILE);
    TileDrawer *drawer = tile->getDrawer();
    drawer->setPos(x*IMAGE_SIZE, y*IMAGE_SIZE);

    inactive_ammo_buckets_[name] = tile;
    map_[name] = tile;
    group_->addToGroup(drawer);
}

void Map::restock_ammo_piles()
{
    std::unordered_map<QString, Tile*> inactive_buckets = get_inactive_ammo_buckets();
    for (const auto &bucket : inactive_buckets) {
        QPair<int, int> coords = map_[bucket.first]->get_coords();
        remove_tile(bucket.first);

        Tile *tile = new Tile(bucket.first, "../silent-edge/src/images/ammo_bucket.png", coords, Tile::TileType::GROUND);
        TileDrawer *drawer = tile->getDrawer();
        drawer->setPos(coords.first * IMAGE_SIZE, coords.second * IMAGE_SIZE);

        active_ammo_buckets_[bucket.first] = tile;
        map_[bucket.first] = tile;
        group_->addToGroup(drawer);
    }

    inactive_buckets.clear();
}

std::unordered_map<QString, Tile*> Map::get_matrix()
{
    return map_;
}

QGraphicsItemGroup* Map::get_group()
{
    return group_;
}

std::unordered_map<QString, Tile *> Map::get_active_ammo_buckets()
{
    return active_ammo_buckets_;
}

std::unordered_map<QString, Tile *> Map::get_inactive_ammo_buckets()
{
    return inactive_ammo_buckets_;
}
