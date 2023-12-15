#pragma once
#include "../gui/overlay_drawer.hpp"

#include <QGraphicsPixmapItem>

class Overlay : QObject
{
public:
    Overlay(int id = 0, const QString &path = "", QPair<int, int> coords = QPair<int, int>(0, 0) ,QObject * = nullptr);
    ~Overlay();
    OverlayDrawer* getDrawer();
    QPair<int, int> get_coords();
    int get_id();
private:
    int id_;
    QPair<int, int> coords_;
    OverlayDrawer *drawer_;
};
