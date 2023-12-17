#include "map.hpp"

#include <QFile>
#include <QTextStream>

Map::Map()
: map_path_("../silent-edge/src/map/map_matrix.txt"), matrix_()
{
    initialize_matrix();
    drawer_ = new MapDrawer(matrix_);
}

Map::~Map()
{
    active_ammo_buckets_.clear();
    matrix_.clear();
}

void Map::initialize_matrix()
{
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
        n_ = n;
        m_ = m;

        int room_id = 0;
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
                    room_id = symbols.remove(0, 1).toInt();
                    std::pair<int, int> start_coords = {j, i};
                    Room *room = new Room(room_id, start_coords, start_coords);
                    rooms_[room_id] = room;
                }
                else if(symbols.contains("$")) {
                    number = 2;
                    room_id = symbols.remove(0, 1).toInt();
                    std::pair<int, int> end_coords = {j+1, i+1};
                    rooms_[room_id]->set_end_coords(end_coords);
                    rooms_[room_id]->set_width_and_height();
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
                        rooms_[room_id]->add_spawnpoint(room_id, QPair<int, int>(i, j));
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
                int id = j * n + i;
                Tile *tile = new Tile(id, path, std::pair<int, int>(j, i), type);

                if(is_ammo)
                    active_ammo_buckets_[id] = tile;
                is_ammo = false;

                matrix_[id] = tile;
            }
        }

        file.close();
    }
}

void Map::remove_from_active(int id)
{
    inactive_ammo_buckets_[id] = active_ammo_buckets_[id];
    active_ammo_buckets_.erase(id);
}

void Map::add_to_active(int id)
{
    active_ammo_buckets_[id] = inactive_ammo_buckets_[id];
    inactive_ammo_buckets_.erase(id);
}

void Map::restock_ammo_piles()
{
    for (const auto &bucket : inactive_ammo_buckets_) {
        drawer_->change_picture(bucket.second, "../silent-edge/src/images/ammo_bucket.png");
        active_ammo_buckets_[bucket.first] = bucket.second;
    }
}

const std::unordered_map<int, Tile*>& Map::get_matrix() const
{
    return matrix_;
}

const std::unordered_map<int, Tile*>& Map::get_active_ammo_buckets() const
{
    return active_ammo_buckets_;
}

const std::unordered_map<int, Tile*>& Map::get_inactive_ammo_buckets() const
{
    return inactive_ammo_buckets_;
}

const std::unordered_map<int, Room*>& Map::get_rooms() const
{
    return rooms_;
}

const Room& Map::get_room_by_id(int id) const
{
    return *rooms_.at(id);
}

Room* Map::add_player_to_a_room(Player *player)
{
    qreal player_x = player->getDrawer()->x();
    qreal player_y = player->getDrawer()->y();

    for(auto &[_, room] : rooms_) {
        int start_x, start_y, end_x, end_y;
        std::tie(start_x, start_y) = room->get_start_coords();
        std::tie(end_x, end_y) = room->get_end_coords();

        if(player_x >= start_x*IMAGE_SIZE && player_x <= end_x*IMAGE_SIZE &&
           player_y >= start_y*IMAGE_SIZE && player_y <= end_y*IMAGE_SIZE) {
            if (!room->get_players_in_room().contains(player))
                room->add_player_to_room(player);
            return room;
        }
    }

    return new Room();
}

MapDrawer* Map::get_drawer()
{
    return drawer_;
}

int Map::get_n()
{
    return n_;
}

int Map::get_m()
{
    return m_;
}
