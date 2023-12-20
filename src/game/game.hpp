#pragma once

#include "../server/server.hpp"
#include "../gui/game_window.hpp"
#include "../client/client_manager.hpp"
#include "game_logic_handler.hpp"
#include "../entity/player.hpp"
#include "../map/map.hpp"
#include "../gui/results.hpp"

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
public slots:
    void playerMoved(QVariant);
    void updateMap(QVariant variant);
    void tileDeleted(int id);
    void bulletMoved(QVariant);
    void showRangList();
signals:
    void roundIsOver();
    void gameIsOver();
private:
    GameServer* server_;
    Client *client_;
    GameWindow *gui_;
    GameLogicHandler* logic_handler_;
    Map *map_;
    std::map<QString, Player*> enemies_;
    QList<std::pair<QString, int>> playerList;
    Results *results_;
};
