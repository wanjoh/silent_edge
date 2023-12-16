#pragma once

#include <QHostAddress>

namespace ServerConfig {
    // za testiranje sa vise racunara
    static constexpr bool REMOTE_SERVER = false;
    inline static const QHostAddress getRemoteIp() { return QHostAddress("192.168.0.23"); };

    static constexpr qint32 PORT = 6969;
    static constexpr qint32 MAX_USERS = 8;
    static constexpr qint32 MAX_ROOMS = MAX_USERS / 2;
    inline static const QHostAddress getHost() {
        return REMOTE_SERVER ? getRemoteIp() : QHostAddress::LocalHost;
    }
    static constexpr quint32 TICK_TIME = 1000 / 30;
}
