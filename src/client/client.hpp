#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QNetworkInterface>
#include "../../src/serialization/binary_serializer.hpp"
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

public slots:
    void connectToServer(const QString &ipAdress, quint16 port);
    // void login(const QString &userName);
    void sendMessage(const QString &text);
    void disconnectFromHost();
private slots:
    void onReadyRead();
signals:
    void connected();
    void disconnected();
    void messageReceived(const QString &sender, const QString &text);
    void error(QAbstractSocket::SocketError socketError);
private:
    QTcpSocket *client_socket_;
    bool logged_in_;
    //void jsonReceived(const QJsonObject &doc);
    void dataReceived(const QByteArray &data);
    float player_x_;
    float player_y_;
    BinarySerializer *serializer_;
    Player *player_;
};
