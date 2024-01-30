#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QNetworkInterface>

class QHostAddress;
class QJsonDocument;
class Client : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Client)
public:
    explicit Client(QObject *parent = nullptr);
    ~Client() override;

    void setPlayerX(float pos_x);
    void setPlayerY(float pos_y);

    void connectToServer(const QString &ipAdress, quint16 port);
    void sendMessage(const QByteArray&);
    void disconnectFromHost();
private slots:
    void onReadyRead();
signals:
    void serverTickReceived();
    void signalTileNameReceived();
    void error(QAbstractSocket::SocketError socketError);
    void dataReceived(const QByteArray &data);
private:
    QTcpSocket *client_socket_;
    bool logged_in_;
};
