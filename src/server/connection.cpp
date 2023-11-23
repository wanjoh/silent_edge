#include "connection.hpp"

#include <QByteArray>

ConnectionThread::ConnectionThread(qintptr socket_desc, QObject * parent)
    :QObject(parent),
    username_("TODO"),
    socket_descriptor_(socket_desc)
{
    if (!(socket_ = new QTcpSocket()))
    {
        qDebug() << "Creating socket failed";
    }
    else
    {
        socket_stream_ = new QDataStream(socket_);
        socket_stream_->setVersion(QDataStream::Qt_6_4);
        socket_->setSocketDescriptor(socket_descriptor_);

        connect(socket_, &QTcpSocket::readyRead, this, &ConnectionThread::receiveData);
        connect(socket_, &QTcpSocket::disconnected, this, &ConnectionThread::disconnectedFromClient);
        connect(socket_, &QTcpSocket::disconnected, this, &ConnectionThread::disconnectClient);
        connect(socket_, &QAbstractSocket::errorOccurred, this, &ConnectionThread::error);
    }
}

ConnectionThread::~ConnectionThread()
{
    socket_stream_ = nullptr;
    socket_->deleteLater();
    socket_descriptor_ = 0;
}

void ConnectionThread::sendData(const QByteArray &data)
{
    *socket_stream_ << data;
}

void ConnectionThread::receiveData()
{
    QByteArray data;
    socket_stream_->startTransaction();
    *socket_stream_ >> data;
    if (socket_stream_->commitTransaction())
    {
        emit dataReceived(data);
    }
}

void ConnectionThread::disconnectClient()
{
    socket_->disconnectFromHost();
    qDebug() << "client disconnected";
}

QString ConnectionThread::username() const
{
    return username_;
}
