#include "overlay.hpp"

Overlay::Overlay(int id, const QString &path, QPair<int, int> coords, QObject *parent)
{
    drawer_ = new OverlayDrawer(path);
    coords_ = coords;
    id_ = id;
}

Overlay::~Overlay()
{

}

int Overlay::get_id()
{
    return id_;
}

QPair<int, int> Overlay::get_coords()
{
    return coords_;
}

OverlayDrawer* Overlay::getDrawer()
{
    return drawer_;
}
