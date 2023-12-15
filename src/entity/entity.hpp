#pragma once

#include "../gui/entity_drawer.hpp"
#include <QVariant>

class Entity : public QObject
{
    Q_OBJECT
public:
    // todo: obrisati ovaj kontruktor
    Entity(QObject* = nullptr);
    Entity(QString, QObject* = nullptr);
    ~Entity() = default;
    QString getName();
    EntityDrawer* getDrawer();
    QString getEtityType();
    virtual QVariant toVariant() const = 0;
    virtual void fromVariant(const QVariant&) = 0;
protected:
    EntityDrawer* drawer_;
    QString name_;
    QString entityType_;
};
