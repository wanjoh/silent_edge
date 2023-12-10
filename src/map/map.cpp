#include "map.hpp"
#include "tile.hpp"

#include <QFile>
#include <QTextStream>

constexpr static qint32 IMAGE_SIZE = 64;

Map::Map()
{
    // uvek isti
    map_path_ = "../silent-edge/src/map/map_matrix.txt";
    matrix_ = make_matrix();
}

QGraphicsItemGroup* Map::draw_matrix()
{
    QGraphicsItemGroup *group = new QGraphicsItemGroup;

    QFile file(map_path_);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        int n, m;
        stream >> n >> m;

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                int image_code;
                stream >> image_code;

                Tile *second_tile;
                TileDrawer *second_drawer;

                QString path = "../silent-edge/src/images/";
                QString name = QString("%1 %2").arg(i).arg(j);
                switch(image_code) {
                    case 0:
                        path += "big_ground.png";
                        break;
                    case 1:
                        path += "big_ground.png";
                        break;
                    case 2:
                        path += "big_wall.png";
                        break;
                    case 3:
                        path += "big_ground.png";

                        second_tile = new Tile("../silent-edge/src/images/spawn_point.png");
                        second_drawer = second_tile->getDrawer();
                        second_drawer->setPos(j*IMAGE_SIZE, i*IMAGE_SIZE);

                        spawnpoints_[name] = second_drawer;
                        break;
                    case 4:
                        path += "big_ground.png";

                        second_tile = new Tile("../silent-edge/src/images/ammo_bucket.png");
                        second_drawer = second_tile->getDrawer();
                        second_drawer->setPos(j*IMAGE_SIZE, i*IMAGE_SIZE);

                        ammo_piles_[name] = second_drawer;
                        break;
                    default:
                        path += "big_ground.png";
                        break;
                }

                Tile *tile = new Tile(path);
                TileDrawer *drawer = tile->getDrawer();
                drawer->setPos(j*IMAGE_SIZE, i*IMAGE_SIZE);
                group->addToGroup(drawer);
            }
        }

        file.close();
    }

    return group;
}

QSharedPointer<QVector<QVector<int>>> Map::make_matrix()
{
    QSharedPointer<QVector<QVector<int>>> matrix = QSharedPointer<QVector<QVector<int>>>(new QVector<QVector<int>>);

    QFile file(map_path_);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        int n, m;
        stream >> n >> m;

        for (int i = 0; i < n; i++)
        {
            QVector<int> row;
            for (int j = 0; j < m; j++)
            {
                int number;
                stream >> number;
                row.push_back(number);
            }
            matrix->push_back(row);
        }

        file.close();
    }

    return matrix;
}

void Map::remove_name_from_ammo_list(QString name)
{
    ammo_piles_.erase(name);
}

QString Map::get_name(int x, int y)
{
    return QString("%1 %2").arg(x).arg(y);
}

QSharedPointer<QVector<QVector<int>>> Map::get_matrix()
{
    return matrix_;
}

std::map<QString, TileDrawer*> Map::get_spawnpoints()
{
    return spawnpoints_;
}

std::map<QString, TileDrawer*> Map::get_ammo_piles()
{
    return ammo_piles_;
}
