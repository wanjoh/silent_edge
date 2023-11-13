#pragma once

#include "game_serialization_global.hpp"
#include "serializable.hpp"

class GameSerializationExport Serializer
{
public:
    virtual ~Serializer() = default;

    virtual void save(const Serializable& serializable, const QString& filepath, const QString& rootName = "") = 0;
    virtual void load(Serializable& serializable, const QString& filepath) = 0;
};
