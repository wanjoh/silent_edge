#include "map.hpp"

#include <QFile>
#include <QTextStream>

Map::Map()
: map_path_("../silent-edge/src/map/map_matrix.txt"), matrix_()
{
    initializeMatrix();
    drawer_ = new MapDrawer(matrix_);
}

Map::~Map()
{
    active_ammo_buckets_.clear();
    matrix_.clear();
}

void Map::initializeMatrix()
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
                if(symbols.contains("^"))
                {
                    number = 2;
                    room_id = symbols.remove(0, 1).toInt();
                    std::pair<int, int> start_coords = {j, i};
                    Room *room = new Room(room_id, start_coords, start_coords);
                    rooms_[room_id] = room;
                }
                else if(symbols.contains("$"))
                {
                    number = 2;
                    room_id = symbols.remove(0, 1).toInt();
                    std::pair<int, int> end_coords = {j+1, i+1};
                    rooms_[room_id]->setEndCoords(end_coords);
                    rooms_[room_id]->setWidthAndHeight();
                }
                else
                {
                    number = symbols.toInt();
                }

                int rand_index = 0;//rand() % 12;

                QString path = "../silent-edge/src/images/";
                Tile::TileType type = Tile::TileType::GROUND;
                switch(number)
                {
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
                        rooms_[room_id]->addSpawnpoint(room_id, QPair<int, int>(i, j));
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

void Map::removeFromActive(int id)
{
    inactive_ammo_buckets_[id] = active_ammo_buckets_[id];
    active_ammo_buckets_.erase(id);
}

void Map::addToActive(int id)
{
    active_ammo_buckets_[id] = inactive_ammo_buckets_[id];
    inactive_ammo_buckets_.erase(id);
}

void Map::restockAmmoPiles()
{
    for (const auto &bucket : inactive_ammo_buckets_) {
        drawer_->change_picture(bucket.second, "../silent-edge/src/images/ammo_bucket.png");
        active_ammo_buckets_[bucket.first] = bucket.second;
    }
}

const std::unordered_map<int, Tile*>& Map::getMatrix() const
{
    return matrix_;
}

const std::unordered_map<int, Tile*>& Map::getActiveAmmoBuckets() const
{
    return active_ammo_buckets_;
}

const std::unordered_map<int, Tile*>& Map::getInactiveAmmoBuckets() const
{
    return inactive_ammo_buckets_;
}

const std::unordered_map<int, Room*>& Map::getRooms() const
{
    return rooms_;
}

const Room& Map::getRoomById(int id) const
{
    return *rooms_.at(id);
}

Room* Map::addPlayerToARoom(Player *player)
{
    qreal player_x = player->getDrawer()->x();
    qreal player_y = player->getDrawer()->y();

    for(auto &[_, room] : rooms_)
    {
        int start_x, start_y, end_x, end_y;
        std::tie(start_x, start_y) = room->getStartCoords();
        std::tie(end_x, end_y) = room->getEndCoords();

        if(player_x >= start_x*IMAGE_SIZE && player_x <= end_x*IMAGE_SIZE &&
           player_y >= start_y*IMAGE_SIZE && player_y <= end_y*IMAGE_SIZE) {
            if (!room->getPlayersInRoom().contains(player))
                room->addPlayerToRoom(player);
            return room;
        }
    }

    return new Room();
}

MapDrawer* Map::getDrawer()
{
    return drawer_;
}

int Map::getN()
{
    return n_;
}

int Map::getM()
{
    return m_;
}
