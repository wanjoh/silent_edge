#pragma once

#include "../gui/entity_drawer.hpp"

class Entity : public QObject
{
    Q_OBJECT
public:
    // todo: obrisati ovaj kontruktor
    // limun: što?
    Entity(QObject* = nullptr);
    Entity(QString, QObject* = nullptr);
    virtual ~Entity() = default;
    QString getName();
    EntityDrawer* getDrawer();
    void setDrawer(EntityDrawer*);
    QString getEtityType();
protected:
    EntityDrawer* drawer_;
    QString name_;
    QString entityType_;
};
