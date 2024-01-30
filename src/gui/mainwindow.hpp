#pragma once

#include <QMainWindow>
#include "../server/server.hpp"
#include "lobby.hpp"
#include "../client/client_manager.hpp"
#include "../game/game.hpp"
#include <QHostAddress>
#include <QString>
#include <QRandomGenerator>
#include <QTime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void onPbCreateServer_clicked();
    void onPbJoinGame_clicked();
    void onPbConnect_clicked();
    void onPbSettings_clicked();
    void onPbBack_clicked();
    void onPbDone_clicked();
    void onPbQuit_clicked();
    void disconnectFromServer();

signals:
    void updateLobbySignal(const QString&);
private:
    Ui::MainWindow *ui;
    GameServer *server_;
    QString username_;
    Client *client_;
    Lobby *lobby_;
};
