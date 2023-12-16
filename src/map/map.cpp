#include "map.hpp"

#include <QFile>
#include <QTextStream>

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

std::unordered_map<int, Tile*> Map::initialize_matrix()
{
    std::unordered_map<int, Tile*> *map = new std::unordered_map<int, Tile*>;

    QVector<QString> wall_paths = {"wall.png", "wall1.png", "wall2.png", "wall3.png", "wall4.png", "wall5.png",
                                    "wall6.png", "wall7.png", "wall8.png", "wall9.png", "wall10.png", "wall11.png",};
    int rand_index;
    srand(time(nullptr));

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
                QString symbols;
                stream >> symbols;

                int number;
                if(symbols.contains("^")) {
                    number = 2;
                    int room_id = symbols.remove(0, 1).toInt();
                    std::pair<int, int> start_coords = {j, i};
                    Room room = *new Room(room_id, start_coords, start_coords);
                    rooms_[room_id] = room;
                }
                else if(symbols.contains("$")) {
                    number = 2;
                    int room_id = symbols.remove(0, 1).toInt();
                    std::pair<int, int> end_coords = {j+1, i+1};
                    rooms_[room_id].set_end_coords(end_coords);
                    rooms_[room_id].set_width_and_height();
                }
                else {
                    number = symbols.toInt();
                }

                int rand_index = 0;//rand() % 12;

                QString path = "../silent-edge/src/images/";
                Tile::TileType type = Tile::TileType::GROUND;
                switch(number) {
                    // barrier
                    case 0:
                        path += "barrier.png";
                        type = Tile::TileType::WALL;
                        break;
                    // ground
                    case 1:
                        path += "ground.png";
                        break;
                    // walls
                    case 2:
                        path += "walls/";
                        path += wall_paths[rand_index];
                        type = Tile::TileType::WALL;
                        break;
                    // spawnpoint
                    case 3:
                        path += "spawnpoint.png";
                        break;
                    // ammo
                    case 4:
                        path += "ammo_bucket.png";
                        type = Tile::TileType::AMMO_PILE;
                        is_ammo = true;
                        break;
                    // default = ground
                    default:
                        path += "ground.png";
                        break;
                }
                int tile_id = j * n + i;
                Tile *tile = new Tile(tile_id, path, std::pair<int, int>(j, i), type);
                TileDrawer *drawer = tile->getDrawer();
                drawer->setPos(j*IMAGE_SIZE, i*IMAGE_SIZE);

                (*map)[tile_id] = tile;
                group_->addToGroup(drawer);

                if(is_ammo)
                    active_ammo_buckets_[tile_id] = tile;

                is_ammo = false;
            }
        }

        file.close();
    }

    return *map;
}

void Map::remove_from_active(int id)
{
    inactive_ammo_buckets_[id] = active_ammo_buckets_[id];
    active_ammo_buckets_.erase(id);
}

void Map::restock_ammo_piles()
{
    std::unordered_map<int, Tile*> inactive_buckets = get_inactive_ammo_buckets();
    for (const auto &bucket : inactive_buckets) {
        bucket.second->setDrawer("../silent-edge/src/images/ammo_bucket.png");
        active_ammo_buckets_[bucket.first] = bucket.second;
    }

    inactive_buckets.clear();
}

std::unordered_map<int, Tile*> Map::get_matrix()
{
    return map_;
}

QGraphicsItemGroup* Map::get_group()
{
    return group_;
}

std::unordered_map<int, Tile *> Map::get_active_ammo_buckets()
{
    return active_ammo_buckets_;
}

std::unordered_map<int, Tile *> Map::get_inactive_ammo_buckets()
{
    return inactive_ammo_buckets_;
}

std::unordered_map<int, Room> Map::get_rooms()
{
    return rooms_;
}

Room Map::get_room_by_id(int id)
{
    return rooms_[id];
}

Room* Map::add_player_to_a_room(EntityDrawer *player)
{
    qreal player_x = player->x();
    qreal player_y = player->y();

    for(auto &[_, room] : rooms_) {
        int start_x, start_y, end_x, end_y;
        std::tie(start_x, start_y) = room.get_start_coords();
        std::tie(end_x, end_y) = room.get_end_coords();

        if(player_x >= start_x*IMAGE_SIZE && player_x <= end_x*IMAGE_SIZE &&
           player_y >= start_y*IMAGE_SIZE && player_y <= end_y*IMAGE_SIZE) {
            if (!room.get_players_in_room().contains(player))
                room.add_player_to_room(player);
            return &room;
        }
    }

    return new Room();
}
