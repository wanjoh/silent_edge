#pragma once

#include "connection.hpp"

#include <QTcpServer>
#include <QVector>
#include <QFile>
#include <QDir>

class QByteArray;

class GameServer : public QTcpServer {
  Q_OBJECT
public:
    // za testiranje sa vise racunara
    static constexpr bool remoteServer = false;
    inline static const QHostAddress remoteIP = QHostAddress("192.168.0.23");
    //
    static constexpr qint32 PORT = 6969;
    static constexpr qint32 MAX_USERS = 8;
    static constexpr qint32 MAX_ROOMS = MAX_USERS / 2;
    inline static const QHostAddress HOST = remoteServer ? remoteIP : QHostAddress::LocalHost;

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
    void calculateScores();
    void rangList();
signals:
    void logMessage(const QString&);
    void stopAllClients();
    void showRangList();
private:
    void sendData(Connection*, const QByteArray&);
    void incomingConnection(qintptr socket_desc) override;
    // dodati kad napravimo algoritam za uparivanje ljudi u sobe
    // za sad je samo jedna soba
    QVector<QVector<Connection*>> room_users_;

    QVector<QThread *> available_threads_;
    QVector<int> threads_load_;
    QVector<Connection*> users_;
    QList<std::pair<Connection*, int>> player_list_;
};
