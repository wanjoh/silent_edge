#pragma once

#include "../server/server.hpp"
#include "../gui/game_window.hpp"
#include "../client/client_manager.hpp"
#include "../entity/player.hpp"
#include "../map/map.hpp"

class Game : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Game)
public:
    inline static const QString DEFAULT_NAME = "playa";
    explicit Game(QString = DEFAULT_NAME, QObject* = nullptr);
    ~Game();
    void startGame();
    void startServer();
    void quit();
    enum class PlayerActions : quint32
    {
        LEFT = 1,
        UP = 1 << 1,
        RIGHT = 1 << 2,
        DOWN = 1 << 3,
        SHOOT = 1 << 4,
        MELEE = 1 << 5
    };

public slots:
    void playerMoved(QVariant);
    void updateMap(QVariant variant);
private:
    GameServer* server_;
    Client *client_;
    GameWindow *gui_;
    Map *map_;
    std::map<QString, Player*> enemies_;
    Player *player_;
};
