#include "entity.hpp"

#include <utility>

Entity::Entity(QObject* parent) : Entity("dummy", parent)
{

}

Entity::Entity(QString name, QObject* parent) :
    QObject(parent)
    , name_(std::move(name))
{
}

auto Entity::getDrawer() -> EntityDrawer*
{
    return drawer_;
}

void Entity::setDrawer(EntityDrawer* drawer)
{
    drawer_ = drawer;
}

auto Entity::getEtityType() -> QString
{
    return entityType_;
}

auto Entity::getName() -> QString
{
    return name_;
}
