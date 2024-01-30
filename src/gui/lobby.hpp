#pragma once

#include <QMainWindow>
#include <QVariant>

namespace Ui {
class Lobby;
}

class Lobby : public QMainWindow
{
    Q_OBJECT

public:
    explicit Lobby(QString server_ip,QWidget *parent = nullptr);
    ~Lobby() override;

    auto isEmpty() const -> bool;

    void onPbExit_clicked();
    void onPbReady_clicked();
    void updateLobby(const QString&);

signals:
    void closeConnection();
    void startGameSignal(const QString&);

private:
    Ui::Lobby *ui;
    QString server_ip_;
};


