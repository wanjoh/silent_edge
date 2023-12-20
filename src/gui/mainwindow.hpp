#pragma once

#include <QMainWindow>
#include "../server/server.hpp"
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

public slots:

    void onPbCreateServer_clicked();
    void onPbJoinGame_clicked();
    void onPbConnect_clicked();
    void onPbSettings_clicked();
    void onPbBack_clicked();
    void onPbDone_clicked();
    void onPbQuit_clicked();

private:
    Ui::MainWindow *ui;
    GameServer *server_;
    QVector<GameServer*> *server_list_;
};
