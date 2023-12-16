#pragma once

#include "connection.hpp"
#include "game_logic_handler.hpp"

#include <QTcpServer>
#include <QVector>
#include <QTimer>

#include "server_config.hpp"

class QByteArray;

class GameServer : public QTcpServer {
  Q_OBJECT
public:
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
