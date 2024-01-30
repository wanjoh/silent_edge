#include "server.hpp"
#include "server_config.hpp"
#include <QDebug>
#include <QTcpSocket>
#include <QByteArray>
#include <QTimer>
#include <QJsonDocument>
#include<QJsonObject>

/*GameServer::GameServer(QObject *parent)
    : QTcpServer(parent)
    , logic_handler_(new GameLogicHandler)
{
    available_threads_.reserve(ServerConfig::MAX_USERS);
    threads_load_.reserve(ServerConfig::MAX_USERS);

    connect(logic_handler_, &GameLogicHandler::gameIsOver, this, &GameServer::gameIsOver);

    if (!this->listen(ServerConfig::getHost(), ServerConfig::PORT))
    {
        qDebug() << "Could not start the server";
    }
    else
    {
        qDebug() << "Server started on " << ServerConfig::getHost() <<  ", port" << ServerConfig::PORT;
        initializeTimers();
    }
}*/

GameServer::GameServer(const QString &ip, QObject *parent)
    : QTcpServer(parent), logic_handler_(new GameLogicHandler),
    server_address_(ip)
{
    available_threads_.reserve(ServerConfig::MAX_USERS);
    threads_load_.reserve(ServerConfig::MAX_USERS);

    connect(logic_handler_, &GameLogicHandler::gameIsOver, this, &GameServer::gameIsOver);

    const QHostAddress ip_address = QHostAddress(ip);

    if (!this->listen(ip_address,ServerConfig::PORT))
    {
        qDebug() << "Could not start the server";
    }
    else
    {
        qDebug() << "Server started on " << ip_address <<  ", port" << ServerConfig::PORT;
        initializeTimers();
    }

    qDebug() << ip_address.toString();
}

GameServer::~GameServer()
{
    for (QThread *singleThread : available_threads_) {
        singleThread->quit();
        singleThread->wait();
    }
    qDebug() << "server gotov";
}

void GameServer::startGame()
{

}

void GameServer::stopServer()
{
    emit stopAllClients();
    close();
}

void GameServer::initializeTimers()
{
    server_timer_.setInterval(ServerConfig::TICK_TIME);
    connect(&server_timer_, &QTimer::timeout, logic_handler_, &GameLogicHandler::updateEntities);
    connect(&server_timer_, &QTimer::timeout, this, &GameServer::emitTickMessage);
    connect(logic_handler_, &GameLogicHandler::updatePlayersSignal, this, &GameServer::broadcast);
    connect(logic_handler_, &GameLogicHandler::updateBulletsSignal, this, &GameServer::broadcast);

    server_timer_.start();
}

void GameServer::error(QTcpSocket::SocketError error)
{
    emit logMessage(QLatin1String("error"));
}

void GameServer::userDisconnected(Connection* user, int thread_idx)
{
    QMutexLocker locker(&users_mutex_);
    QMutexLocker lobby_locker(&server_lobbies_mutex_);
    Q_ASSERT(user);
    // videti da li nam ovo treba i kako ga azurirati
//    --threads_load_[thread_idx];
    users_.removeAll(user);
//    user_to_server_map_.remove(user);
//    if (server_lobbies_[user_to_server_map_[user]]->isEmpty()) {
//        delete server_lobbies_[user_to_server_map_[user]];
//    }
}

void GameServer::broadcast(const QByteArray& msg)
{
    for (auto *user : users_) {
        Q_ASSERT(user);
        user->sendData(msg);
    }
}

void GameServer::sendData(Connection *user, const QByteArray& msg)
{
    Q_ASSERT(user);

    QTimer::singleShot(0, user, [user, msg] { user->sendData(msg); });
}

void GameServer::incomingConnection(qintptr socket_desc)
{
    //QMutexLocker lobby_locker(&server_lobbies_mutex_);
    if (users_.size() < ServerConfig::MAX_USERS)
    {
        // ovo treba da se razmotri, nismo sigurni da li će ovo praviti siročiće
        auto *user = new Connection(socket_desc, nullptr);

        int thread_idx = available_threads_.size();

        if (thread_idx < ServerConfig::MAX_USERS)
        {
            available_threads_.append(new QThread(this));
            available_threads_.last()->start();
        }
        else
        {
            delete user;
            qDebug() << "Previse korisnika povezano";
            return;
        }

        user->moveToThread(available_threads_.at(thread_idx));

        connect(user, &Connection::disconnectedFromServer, this, [this, user, thread_idx] {
            userDisconnected(user, thread_idx);
        });
        connect(user, &Connection::error, this, [this](auto &&PH1) {
            error(std::forward<decltype(PH1)>(PH1));
        });
        connect(user, &Connection::dataReceived, logic_handler_, &GameLogicHandler::updatePlayerStats);
        connect(user, &Connection::logMessage, this, &GameServer::logMessage);
        connect(this, &GameServer::stopAllClients, user, &Connection::disconnectedFromServer); // popraviti

        connect(user, &Connection::disconnectedFromServer, available_threads_.last(), &QThread::quit);
        connect(user, &Connection::disconnectedFromServer, user, &Connection::deleteLater);
        connect(available_threads_.last(), &QThread::finished, user, &QObject::deleteLater);

        users_.push_back(user);

        qDebug() << "thread created";
    }
}

void GameServer::emitTickMessage()
{
    broadcast(QByteArray("tick\0"));
}

auto GameServer::getLobby() const -> Lobby *
{
    return lobby;
}

auto GameServer::server_address() const -> QString
{
    return server_address_;
}

void GameServer::gameIsOver()
{
    qDebug() << "rang";
//    emit showResults();
}
