#pragma once

#include "connection.hpp"
#include "../gui/lobby.hpp"
#include <QTcpServer>
#include <QVector>

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
    //static QHostAddress HOST;
    inline static const QHostAddress HOST = remoteServer ? remoteIP : QHostAddress::LocalHost;
    Lobby *lobby;
    Lobby* getServerLobby(const QString&) const;

    GameServer(QObject* = nullptr);
    GameServer(QString,QObject* = nullptr);
    GameServer(const GameServer&) = delete;
    ~GameServer();

    void startGame();
    QString server_address() const;

public slots:
    void error(QTcpSocket::SocketError);
    void dataReceived(Connection*, const QByteArray&);
    void userDisconnected(Connection*, int);
    void broadcast(const QByteArray&, Connection*);
    void stopServer();
signals:
    void logMessage(const QString&);
    void stopAllClients();
    void playerJoined(const QString&, Lobby* lobby);
private:
    void sendData(Connection*, const QByteArray&);
    void incomingConnection(qintptr socket_desc) override;
    // dodati kad napravimo algoritam za uparivanje ljudi u sobe
    // za sad je samo jedna soba
    //QVector<QVector<Connection*>> room_users_;
    QMap<Connection*, QString> user_servers_;
    QMap<QString, Lobby*> server_lobbies_;

    QVector<QThread *> available_threads_;
    QVector<int> threads_load_;
    QVector<Connection*> users_;
    QString server_address_;
    QMutex users_mutex_;
    QMutex server_lobbies_mutex_;
};
