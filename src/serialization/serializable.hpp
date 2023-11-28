#pragma once

#include <QVariant>

class Serializable
{
public:
    Serializable();
    ~Serializable() = default;

    virtual QVariant toVariant() const = 0;
    virtual void fromVariant(const QVariant&) = 0;
};
