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
    ~Entity() override = default;
    auto getName() -> QString;
    auto getDrawer() -> EntityDrawer *;
    void setDrawer(EntityDrawer*);
    auto getEtityType() -> QString;

protected:
    EntityDrawer* drawer_;
    QString name_;
    QString entityType_;
};
