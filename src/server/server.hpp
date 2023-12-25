#pragma once

#include "connection.hpp"
#include "game_logic_handler.hpp"

#include <QTcpServer>
#include <QVector>

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
    void userDisconnected(Connection*, int);
    void broadcast(const QByteArray&);
    void stopServer();
    void updatePlayersSignal(const QByteArray&);
    void updateBulletsSignal(const QByteArray&);
private slots:
    void emitTickMessage();
signals:
    void logMessage(const QString&);
    void stopAllClients();
private:
    void initializeTimers();
    void sendData(Connection*, const QByteArray&);
    void incomingConnection(qintptr socket_desc) override;
    GameLogicHandler* logic_handler_;
    QVector<QThread *> available_threads_;
    QVector<int> threads_load_;
    QVector<Connection*> users_;
    QTimer server_timer_;
    QVector<QByteArray> player_datas_;
};
