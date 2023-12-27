#include "entity.hpp"

Entity::Entity(QObject* parent) : Entity("dummy", parent)
{

}

Entity::Entity(QString name, QObject* parent) :
    QObject(parent)
    , name_(name)
{
}

EntityDrawer* Entity::getDrawer()
{
    return drawer_;
}

QString Entity::getEtityType()
{
    return entityType_;
}

QString Entity::getName()
{
    return name_;
}
