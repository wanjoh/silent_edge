#include "player.hpp"

#include <QKeyEvent>
//TODO: OCISTITI DJUBRE

void Player::keyPressEvent(QKeyEvent *event)
{
    keys_[event->key()] = true;

}

void Player::keyReleaseEvent(QKeyEvent *event)
{
    keys_[event->key()] = false;

}


void Player::updateMovement()
{
    qreal x_pos = x();
    qreal y_pos = y();

    bool moved = false;

    qreal dx = (keys_[Qt::Key_D] ? 1.0 : 0.0) - (keys_[Qt::Key_A] ? 1.0 : 0.0);
    qreal dy = (keys_[Qt::Key_S] ? 1.0 : 0.0) - (keys_[Qt::Key_W] ? 1.0 : 0.0);

    if (dx != 0.0 && dy != 0.0) {
        qreal length = qSqrt(dx * dx + dy * dy);
        dx /= length;
        dy /= length;
    }

    x_pos += dx * Player::VELOCITY;
    y_pos += dy * Player::VELOCITY;

    if (dx != 0.0 || dy != 0.0) {
        moved = true;
    }

    if (moved) {
        setPos(x_pos, y_pos);

        emit positionChangedSignal();
    }
}

void Player::initializeTimer()
{
    movementTimer_.setInterval(16);  // Approximately 60 FPS
    connect(&movementTimer_, &QTimer::timeout, this, &Player::updateMovement);
    movementTimer_.start();
}

Player::Player(QString name) : name_(name)
{
    initializeTimer();
}

Player::Player(QString name, qreal x, qreal y) : name_(name)
{
    initializeTimer();
    setX(x);
    setY(y);
}

QVariant Player::toVariant() const
{
    QVariantMap map;
    map.insert("name", name_);
    map.insert("position_x", x());
    map.insert("position_y", y());
    //qDebug() << "to variant" << map;
    //map.insert("hp", hp_);

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
    //qDebug() << "from variant" << x << " " << y << " " << name;
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
