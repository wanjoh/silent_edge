#include "map.hpp"

#include <QFile>
#include <QTextStream>

Map::Map()
    : map_path_("../silent-edge/src/map/map_matrix.txt")
    , matrix_()
{
    initializeMatrix();
    drawer_ = new MapDrawer(matrix_);
}

Map::~Map()
{
    for (auto &[name, bucket] : active_ammo_buckets_) {
        active_ammo_buckets_.erase(name);
    }
    active_ammo_buckets_.clear();

    for (auto &[id, tile] : matrix_) {
        matrix_.erase(id);
    }
    matrix_.clear();
}

void Map::initializeMatrix()
{
    QVector<QString> wall_paths = {"wall.png", "wall1.png", "wall2.png", "wall3.png", "wall4.png", "wall5.png",
                                    "wall6.png", "wall7.png", "wall8.png", "wall9.png", "wall10.png", "wall11.png",};
    srand(time(nullptr));

    QFile file(map_path_);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        int n;
        int m;
        stream >> n >> m;
        n_ = n;
        m_ = m;

        int room_id = 0;
        float is_ammo = 0.0f;
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

                int number_of_spawnpoints;
                QString spawnpoint_name;

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
                        number_of_spawnpoints = rooms_[room_id]->getUnusedSpawnpoints().size();
                        spawnpoint_name = QString::number(room_id).append(" ").append(QString::number(number_of_spawnpoints));
                        rooms_[room_id]->addSpawnpoint(spawnpoint_name, QPair<int, int>(i, j));
                        break;
                    // ammo
                    case 4:
                        path += "ammo_bucket.png";
                        type = Tile::TileType::AMMO_PILE;
                        is_ammo = 1.0f;
                        break;
                    // default = ground
                    default:
                        path += "ground.png";
                        break;
                }
                int id = i * m + j;
                Tile *tile = new Tile(id, path, std::pair<int, int>(j, i), type);

                if (is_ammo != 0.0f) {
                    active_ammo_buckets_[id] = tile;
                }
                is_ammo = 0.0f;

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
        drawer_->change_picture(bucket.first, "../silent-edge/src/images/ammo_bucket.png");
        active_ammo_buckets_[bucket.first] = bucket.second;
    }
}

auto Map::getMatrix() const -> const std::unordered_map<int, Tile*>&
{
    return matrix_;
}

auto Map::getActiveAmmoBuckets() const -> const std::unordered_map<int, Tile*>&
{
    return active_ammo_buckets_;
}

auto Map::getInactiveAmmoBuckets() const -> const std::unordered_map<int, Tile*>&
{
    return inactive_ammo_buckets_;
}

auto Map::getRooms() const -> const std::unordered_map<int, Room*>&
{
    return rooms_;
}

auto Map::getRoomById(int id) const -> const Room&
{
    return *rooms_.at(id);
}

auto Map::findRoomForPlayer(Player& player) -> Room*
{
    qreal player_x = player.getDrawer()->x();
    qreal player_y = player.getDrawer()->y();

    for(auto &[_, room] : rooms_)
    {
        QVector<Player*> players = room->getPlayersInRoom();
        if (players.contains(&player)) {
            players.removeOne(&player);
        }
    }

    for(auto &[_, room] : rooms_)
    {
        int start_x;
        int start_y;
        int end_x;
        int end_y;
        std::tie(start_x, start_y) = room->getStartCoords();
        std::tie(end_x, end_y) = room->getEndCoords();

        if(player_x >= start_x*IMAGE_SIZE && player_x <= end_x*IMAGE_SIZE &&
           player_y >= start_y*IMAGE_SIZE && player_y <= end_y*IMAGE_SIZE) {
            if (!room->getPlayersInRoom().contains(&player)) {
                room->addPlayerToRoom(&player);
            }
            return room;
        }
    }

    return new Room();
}

auto Map::getDrawer() -> MapDrawer*
{
    return drawer_;
}

auto Map::getN() const -> int
{
    return n_;
}

auto Map::getM() const -> int
{
    return m_;
}
