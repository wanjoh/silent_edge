#include "map.hpp"
#include "../gui/entity_drawer.hpp"

#include <QFile>
#include <QTextStream>

constexpr static qint32 IMAGE_SIZE = 64;

Map::Map(QString* path_to_map)
{

}

QGraphicsItemGroup* Map::draw_matrix(QString* path_to_map)
{
    QGraphicsItemGroup *group = new QGraphicsItemGroup;

    QFile file(*path_to_map);
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

                QString path = "";
                switch(image_code) {
                    case 0:
                        path = "";
                        break;
                    case 1:
                        path = "../silent-edge/src/images/big_ground.png";
                        break;
                    case 2:
                        path = "../silent-edge/src/images/big_wall.png";
                        break;
                }
                EntityDrawer *tile = new EntityDrawer(path);
                tile->setPos(j*IMAGE_SIZE, i*IMAGE_SIZE);
                group->addToGroup(tile);
            }
        }

        file.close();
    }

    return group;
}

QSharedPointer<QVector<QVector<int>>> Map::get_matrix(QString* path_to_map)
{
    QSharedPointer<QVector<QVector<int>>> matrix = QSharedPointer<QVector<QVector<int>>>(new QVector<QVector<int>>);

    QFile file(*path_to_map);
    // podrazumevam da se fajl moze otvoriti...
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
