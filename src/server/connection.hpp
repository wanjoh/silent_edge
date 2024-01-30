#pragma once

#include <QThread>
#include <QTcpSocket>
#include <QMutex>

class Connection : public QObject
{
    Q_OBJECT
public:
    Connection(qintptr, QObject* = nullptr);
    Connection(const Connection&) = delete;
    ~Connection() override;

    void sendData(const QByteArray&);
    auto username() const -> QString;

    void receiveData();
    void disconnectClient();
signals:
    void error(QTcpSocket::SocketError);
    void logMessage(const QString&);
    void dataReceived(const QByteArray&);
    void disconnectedFromServer();
private:
    QString username_;
    qintptr socket_descriptor_;
    QTcpSocket *socket_;
    QDataStream *socket_stream_;
};
