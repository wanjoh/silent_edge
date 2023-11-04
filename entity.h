#pragma once

#include <QGraphicsRectItem>

class Entity : public QGraphicsRectItem
{
public:
    std::pair<float, float> get_position();
    std::pair<float, float> get_size();
    float getRotation() const;
    void setRotation(float newRotation);

private:
    float rotation;
};
