#pragma once

#include <QVariant>
#include "game_serialization_global.hpp"

class GameSerializationExport Serializable
{
public:
    virtual ~Serializable() = default;
    virtual QVariant toVariant() const = 0;
    virtual void fromVariant(const QVariant & variant) = 0;
};
