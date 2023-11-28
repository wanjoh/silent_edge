#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QNetworkInterface>
#include "../entity/player.hpp"

class QHostAddress;
class QJsonDocument;
class Client : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Client)
public:
    explicit Client(QObject *parent = nullptr);
    ~Client();

    void setPlayerX(float pos_x);
    void setPlayerY(float pos_y);

    float getPlayerX();
    float getPlayerY();
    Player* getPlayer();
public slots:
    void connectToServer(const QString &ipAdress, quint16 port);
    // void login(const QString &userName);
    void sendMessage();
    void disconnectFromHost();
private slots:
    void onReadyRead();
    void updatePosition();
signals:
    void connected();
    void disconnected();
    void signalDataReceived(Player*);
    void error(QAbstractSocket::SocketError socketError);
private:
    QTcpSocket *client_socket_;
    bool logged_in_;
    //void jsonReceived(const QJsonObject &doc);
    void dataReceived(QByteArray &data);
    float player_x_;
    float player_y_;
    Player *player_;
};
