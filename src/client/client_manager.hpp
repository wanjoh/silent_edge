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
    ~Client();

    void setPlayerX(float pos_x);
    void setPlayerY(float pos_y);

public slots:
    void connectToServer(const QString &ipAdress, quint16 port);
    void sendMessage(QString, quint32);
    void disconnectFromHost();
private slots:
    void onReadyRead();
signals:
    void serverTickReceived();
    void signalDataReceived(QVariant);
    void signalTileNameReceived(QVariant);
    void error(QAbstractSocket::SocketError socketError);
private:
    QTcpSocket *client_socket_;
    bool logged_in_;
    //void jsonReceived(const QJsonObject &doc);
    void dataReceived(const QByteArray &data);
};
