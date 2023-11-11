#pragma once

#include "connection_thread.hpp"

#include <QTcpServer>
#include <QVector>

class QJsonObject;

class GameServer : public QTcpServer {
  Q_OBJECT
public:
    static constexpr qint32 PORT = 6969;
    static constexpr qint32 MAX_USERS = 8;
    static constexpr qint32 MAX_ROOMS = MAX_USERS / 2;
    inline static const QHostAddress HOST = QHostAddress::LocalHost;

    GameServer(QObject* = nullptr);
    GameServer(const GameServer&) = delete;
    ~GameServer();

    void startGame();
public slots:
    void error(QTcpSocket::SocketError);
    void dataReceived(ConnectionThread*, const QJsonObject&);
    void userDisconnected(ConnectionThread*);
    void broadcast(const QJsonObject&, ConnectionThread*);
signals:
    void logMessage(const QString&);
private:
    void sendData(ConnectionThread*, const QJsonObject&);
    void incomingConnection(qintptr socket_desc) override;
    // dodati kad napravimo algoritam za uparivanje ljudi u sobe
    // za sad je samo jedna soba
    //QVector<QVector<ConnectionThread*>> room_users_;
    QVector<ConnectionThread*> users_;
};
