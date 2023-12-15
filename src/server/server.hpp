#pragma once

#include "connection.hpp"
#include "game_logic_handler.hpp"

#include <QTcpServer>
#include <QVector>
#include <QTimer>

class QByteArray;

class GameServer : public QTcpServer {
  Q_OBJECT
public:
    // za testiranje sa vise racunara
    static constexpr bool remoteServer = false;
    inline static const QHostAddress remoteIP = QHostAddress("192.168.0.23");
    //
    //todo: ubaciti ovo u neki server config
    static constexpr qint32 PORT = 6969;
    static constexpr qint32 MAX_USERS = 8;
    static constexpr qint32 MAX_ROOMS = MAX_USERS / 2;
    inline static const QHostAddress HOST = remoteServer ? remoteIP : QHostAddress::LocalHost;
    static constexpr quint32 TICK_TIME = 1000 / 30;

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
    void initializeTimers();
    void sendData(Connection*, const QByteArray&);
    void incomingConnection(qintptr socket_desc) override;
    void tick();
    void collectData();
    // dodati kad napravimo algoritam za uparivanje ljudi u sobe
    // za sad je samo jedna soba
    //QVector<QVector<Connection*>> room_users_;
    GameLogicHandler* logic_handler_;
    QVector<QThread *> available_threads_;
    QVector<int> threads_load_;
    QVector<Connection*> users_;
    QTimer server_timer_;
};
