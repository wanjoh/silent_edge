#include "map.hpp"
#include "../gui/entity_drawer.hpp"

#include <QFile>
#include <QTextStream>

constexpr static qint32 IMAGE_SIZE = 32;

Map::Map(QString* path_to_map)
{

}

QGraphicsItemGroup* Map::draw_matrix(QString* path_to_map)
{
    QGraphicsItemGroup *group = new QGraphicsItemGroup;

    QFile file(*path_to_map);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        const int n = stream.read(1).toInt();
        const int m = stream.read(1).toInt();

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                int image_code = stream.read(1).toInt();

                QString path = "";
                switch(image_code) {
                    case 0:
                        path = "";
                        break;
                    case 1:
                        path = "../images/ground.png";
                        break;
                    case 2:
                        path = "../images/wall.png";
                        break;
                }
                EntityDrawer *tile = new EntityDrawer(path);
                tile->setPos(i*IMAGE_SIZE, j*IMAGE_SIZE);
                group->addToGroup(tile);
            }
        }

        file.close();
    }

    return group;
}
