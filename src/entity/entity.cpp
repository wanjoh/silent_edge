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

EntityDrawer* Entity::getDrawer()
{
    return drawer_;
}

void Entity::setDrawer(EntityDrawer* drawer)
{
    drawer_ = drawer;
}

QString Entity::getEtityType()
{
    return entityType_;
}

QString Entity::getName()
{
    return name_;
}
