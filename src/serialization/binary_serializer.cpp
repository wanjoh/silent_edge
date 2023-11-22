#include <QFile>
#include <QDataStream>

#include "binary_serializer.hpp"

BinarySerializer::BinarySerializer() {}

void BinarySerializer::save(const Serializable &serializable, const QString &filepath, const QString &rootName)
{
    Q_UNUSED(rootName);

    QFile file(filepath);
    file.open(QFile::WriteOnly);

    QDataStream dataStream(&file);
    dataStream << serializable.toVariant();

    file.close();
}

void BinarySerializer::load(Serializable &serializable, const QString &filepath)
{
    QFile file(filepath);
    file.open(QFile::ReadOnly);

    QDataStream dataStream(&file);
    QVariant variant;

    dataStream >> variant;
    serializable.fromVariant(variant);

    file.close();
}

