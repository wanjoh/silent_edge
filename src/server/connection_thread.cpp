#include "connection_thread.hpp"

#include <QJsonDocument>
#include <QJsonObject>

ConnectionThread::ConnectionThread(qintptr socket_desc, QObject * parent) //:QThread(parent),
    : QObject(parent),
    connected_(true),
    username_("TODO"),
    socket_(new QTcpSocket()),
    socket_descriptor_(socket_desc),
    socket_stream_(socket_)
{
    socket_stream_.setVersion(QDataStream::Qt_6_4);
    if (!(socket_ = new QTcpSocket(this)))
    {
        qDebug() << "Creating socket failed";
    }
    else
    {
        connect(socket_, &QTcpSocket::readyRead, this, &ConnectionThread::receiveData);
        connect(socket_, &QTcpSocket::disconnected, this, &ConnectionThread::disconnectedFromClient);
        connect(socket_, &QTcpSocket::disconnected, this, &ConnectionThread::disconnectClient);
        connect(socket_, &QAbstractSocket::errorOccurred, this, &ConnectionThread::error);
    }
}

ConnectionThread::~ConnectionThread()
{
    socket_->deleteLater();
    //socket_ = nullptr;
    socket_descriptor_ = 0;
}

void ConnectionThread::sendData(const QJsonObject &json)
{
    const QByteArray data = QJsonDocument(json).toJson(QJsonDocument::Compact);
    socket_stream_ << data;
}

void ConnectionThread::receiveData()
{
    QByteArray json_data;
    while (connected_)
    {
        socket_stream_.startTransaction();
        socket_stream_ >> json_data;
        if (socket_stream_.commitTransaction())
        {
            QJsonParseError error;
            const QJsonDocument data = QJsonDocument::fromJson(json_data, &error);

            if (error.error == QJsonParseError::NoError && data.isObject())
            {
                emit dataReceived(data.object());
            }
            else
            {
                emit logMessage(QLatin1String("Invalid message"));
            }
        }

    }
}

void ConnectionThread::disconnectClient()
{
    socket_->disconnectFromHost();
    connected_ = false;
    qDebug() << "client disconnected";
}

QString ConnectionThread::username() const
{
    return username_;
}
