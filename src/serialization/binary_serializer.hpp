#pragma once

#include "game_serialization_global.hpp"
#include "serializer.hpp"

class GameSerializationExport BinarySerializer : public Serializer
{
public:
    BinarySerializer();

    void save(const Serializable &serializable, const QString &filepath, const QString &rootName = "") override;
    void load(Serializable &serializable, const QString &filepath) override;
};
