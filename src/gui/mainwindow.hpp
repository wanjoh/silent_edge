#pragma once

#include <QMainWindow>
#include "../server/server.hpp"
#include "lobby.hpp"
#include "../client/client_manager.hpp"
#include "../game/game.hpp"
#include <QHostAddress>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool test_connection = true;

public slots:

    void onPbCreateServer_clicked();
    void onPbJoinGame_clicked();
    void onPbConnect_clicked();
    void onPbSettings_clicked();
    void onPbBack_clicked();
    void onPbDone_clicked();
    void onPbQuit_clicked();
    void onPlayerJoined(const QString&, Lobby*);
    void disconnectFromServer();
    QString getLocalIPv4Address();
    void startGame(const QString&);

signals:
    void updateLobbySignal(const QString&);
private:
    Ui::MainWindow *ui;
    GameServer *server_;
    QString username_;
    Client *client_;
    std::unordered_map<QString,GameServer*> list_of_servers;
    Lobby *lobby_;
};
