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

    GameServer(QObject* = nullptr);
    GameServer(QString,QObject* = nullptr);
    GameServer(const GameServer&) = delete;
    ~GameServer();

    void startGame();
    QString server_address() const;

    Lobby *getLobby() const;

public slots:
    void error(QTcpSocket::SocketError);
    void userDisconnected(Connection*, int);
    void broadcast(const QByteArray&);
    void stopServer();
    void updateAllSignal(const QByteArray&, const QByteArray&);
    void tileChangedSignal(const QByteArray&);
    void restockAmmoPilesSignal();
    void receiveRefreshCameraSignal(const QByteArray& refresh_info);

private slots:
    void emitTickMessage();
signals:
    void logMessage(const QString&);
    void removeBulletSignal(QString);
    void reloadItemSignal(QString, EntityDrawer*);
    void meleeSwingSignal(QString, EntityDrawer*);
    void removeReload(QString);
    void removeMelee(QString);
    void labelSignal(qint32, qint32, qint32);

    void stopAllClients();
    void playerJoined(const QString&, Lobby* lobby);
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
    QString server_address_;
    QMutex users_mutex_;
    QMutex server_lobbies_mutex_;
    Lobby *lobby;
    QString username_;

};
