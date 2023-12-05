#pragma once

#include "../serialization/serializable.hpp"
#include "../gui/entity_drawer.hpp"

class Entity : public Serializable, public QObject
{
public:
    Entity(QObject* = nullptr);
    Entity(QString, QObject* = nullptr);
    ~Entity() = default;
    QString getName();
    EntityDrawer* getDrawer();
protected:
    EntityDrawer* drawer_;
    QString name_;
};
