#pragma once

#include "connection.hpp"
#include "game_logic_handler.hpp"
#include "../gui/lobby.hpp"
#include <QTcpServer>
#include <QVector>

class QByteArray;

class GameServer : public QTcpServer {
  Q_OBJECT
public:

    //GameServer(QObject* = nullptr);
    GameServer(const QString &, QObject* = nullptr);
    GameServer(const GameServer&) = delete;
    ~GameServer() override;

    void startGame();
    auto server_address() const -> QString;

    auto getLobby() const -> Lobby *;

    void error(QTcpSocket::SocketError);
    void userDisconnected(Connection*, int);
    void broadcast(const QByteArray&);
    void stopServer();
private slots:
    void emitTickMessage();
    static void gameIsOver();
signals:
    void logMessage(const QString&);
    void stopAllClients();
    void playerJoined(const QString&, Lobby* lobby);
    void showResults();
private:
    void initializeTimers();
    static void sendData(Connection*, const QByteArray&);
    void incomingConnection(qintptr socket_desc) override;
    GameLogicHandler* logic_handler_;
    QVector<QThread *> available_threads_;
    QVector<int> threads_load_;
    QVector<Connection*> users_;
    QTimer server_timer_;
    QVector<QByteArray> player_datas_;
    QString server_address_;
    QMutex users_mutex_;
    QMutex server_lobbies_mutex_;
    Lobby *lobby;
    QString username_;

};
