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
    void addAllDynamicTiles();
    void quit();
public slots:
    void updateEnemy(QVariant);
    void updateMap(QVariant variant);
    void playerMoved();
    void tileDeleted(QString name);
private:
    void initializeTimer();
    GameServer* server_;

    GameWindow *gui_;
    Player *player_;
    Map *map_;

    std::map<QString, Player*> enemies_;
    Client *client_;
};
