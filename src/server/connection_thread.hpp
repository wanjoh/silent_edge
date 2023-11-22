#pragma once

#include <QThread>
#include <QTcpSocket>
#include <QMutex>

class ConnectionThread : public QThread
{
    Q_OBJECT
public:
    ConnectionThread(qintptr, QObject* = nullptr);
    ConnectionThread(const ConnectionThread&) = delete;
    ~ConnectionThread();

    void sendData(const QByteArray&);

    QString username() const;

public slots:
    void receiveData();
    void disconnectClient();
signals:
    void error(QTcpSocket::SocketError);
    void logMessage(const QString&);
    void dataReceived(const QByteArray&);
    void disconnectedFromClient();
private:
    QString username_;
    qintptr socket_descriptor_;
    QTcpSocket *socket_;
    QDataStream *socket_stream_;
    QMutex mutex_;
};
