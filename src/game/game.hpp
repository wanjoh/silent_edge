#pragma once

#include "../server/server.hpp"
#include "../gui/game_window.hpp"
#include "../client/client_manager.hpp"
#include "../entity/player.hpp"
#include "../map/map.hpp"
#include <QJsonObject>

class Game : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Game)
public:
    inline static const QString DEFAULT_NAME = "playa";
    explicit Game(QString = DEFAULT_NAME,Client* = nullptr, QObject* = nullptr);
    ~Game();
    void startGame(QString server_ip);
    void startServer(QString server_ip);
    void quit();

    void deserializeData(const QByteArray &);

private:
    void serializeData();
    GameServer* server_;
    QJsonObject json_object_;
    Client *client_;
    GameWindow *gui_;
    Map *map_;
    std::map<QString, Player*> enemies_;
    std::map<int, EntityDrawer*> bullet_drawers_;
    Player *player_;
};
