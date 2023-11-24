#include "player.hpp"

#include <QKeyEvent>
//TODO: OCISTITI DJUBRE

void Player::keyPressEvent(QKeyEvent *event)
{
    qreal x_pos = x();
    qreal y_pos = y();

    bool moved = true;
    switch(event->key())
    {
    case Qt::Key_W:
        y_pos -= Player::VELOCITY;
        break;
    case Qt::Key_S:
        y_pos += Player::VELOCITY;
        break;
    case Qt::Key_A:
        x_pos -= Player::VELOCITY;
        break;
    case Qt::Key_D:
        x_pos += Player::VELOCITY;
        break;
    default:
        moved = false;
    }

    if(moved) {
        setPos(x_pos, y_pos);
        emit positionChangedSignal();
    }
}

Player::Player(QString name) : name_(name)
{
}

Player::Player(QString name, qreal x, qreal y) : name_(name)
{
    setX(x);
    setY(y);
}

QVariant Player::toVariant() const
{
    QVariantMap map;
    map.insert("name", name_);
    map.insert("position_x", x());
    map.insert("position_y", y());
//    map.insert("hp", hp_);

    return map;
}

void Player::fromVariant(const QVariant& variant)
{
    const auto map = variant.toMap();
    QString name = map.value("name").toString();
    qreal x = map.value("position_x").toReal();
    qreal y = map.value("position_y").toReal();
    setPos(x, y);
    name_ = name;
//    qDebug() << "from variant" << x << " " << y << " " << name;
}

QString Player::getName()
{
    return name_;
}

void Player::setName(QString name)
{
    this->name_ = name;
}

void Player::move(Player* enemy)
{
    setPos(enemy->x(), enemy->y());
}
