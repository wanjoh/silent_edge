#include "player.hpp"

#include <QKeyEvent>

void Player::keyPressEvent(QKeyEvent *event)
{
    qreal x_pos = x();
    qreal y_pos = y();
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
    }

    setPos(x_pos, y_pos);
}
