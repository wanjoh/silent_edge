#pragma once

#include "../gui/entity_drawer.hpp"
#include <QVariant>

class Entity : public QObject
{
    Q_OBJECT
public:
    Entity(QObject* = nullptr);
    Entity(QString, QObject* = nullptr);
    ~Entity() = default;
    QString getName();
    EntityDrawer* getDrawer();
    virtual QVariant toVariant() const = 0;
    virtual void fromVariant(const QVariant&) = 0;
protected:
    EntityDrawer* drawer_;
    QString name_;
};
