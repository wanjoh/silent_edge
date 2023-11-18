#pragma once

#include "serializer.hpp"

class BinarySerializer : public Serializer
{
public:
    BinarySerializer();

    void save(const Serializable &serializable, const QString &filepath, const QString &rootName = "") override;
    void load(Serializable &serializable, const QString &filepath) override;
};
