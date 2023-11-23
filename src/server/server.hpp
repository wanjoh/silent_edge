#pragma once

#include "connection.hpp"

#include <QTcpServer>
#include <QVector>

class QByteArray;

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
    void dataReceived(Connection*, const QByteArray&);
    void userDisconnected(Connection*, int);
    void broadcast(const QByteArray&, Connection*);
    void stopServer();
signals:
    void logMessage(const QString&);
    void stopAllClients();
private:
    void sendData(Connection*, const QByteArray&);
    void incomingConnection(qintptr socket_desc) override;
    // dodati kad napravimo algoritam za uparivanje ljudi u sobe
    // za sad je samo jedna soba
    //QVector<QVector<Connection*>> room_users_;

    QVector<QThread *> available_threads_;
    QVector<int> threads_load_;
    QVector<Connection*> users_;
};
