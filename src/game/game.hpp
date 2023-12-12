#pragma once

#include "../server/server.hpp"
#include "../gui/game_window.hpp"
#include "../client/client_manager.hpp"
#include "game_logic_handler.hpp"

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
    void updateEnemy(QVariant);
    void playerMoved(QVariant);
private:
    void initializeTimer();
    GameServer* server_;

    Client *client_;
    GameWindow *gui_;
    GameLogicHandler* logic_handler_;
    
    std::map<QString, Player*> enemies_;
};
