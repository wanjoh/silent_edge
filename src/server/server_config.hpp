#pragma once

#include <QHostAddress>

namespace ServerConfig {
enum class PlayerActions : quint32
{
    LEFT = 1,
    UP = 1 << 1,
    RIGHT = 1 << 2,
    DOWN = 1 << 3,
    SHOOT = 1 << 4,
    MELEE = 1 << 5,
    RELOAD = 1 << 6
};
inline quint32 operator&(quint32 a, PlayerActions b)
{
    return a & static_cast<quint32>(b);
}


// za testiranje sa vise racunara
static constexpr bool REMOTE_SERVER = true;
inline static const QHostAddress getRemoteIp() { return QHostAddress("192.168.100.49"); };

static constexpr qint32 PORT = 6969;
static constexpr qint32 MAX_USERS = 8;
static constexpr qint32 MAX_ROOMS = MAX_USERS / 2;
inline static const QHostAddress getHost() {
    return REMOTE_SERVER ? getRemoteIp() : QHostAddress::LocalHost;
}
static constexpr quint32 TICK_TIME = 1000 / 60;
}
