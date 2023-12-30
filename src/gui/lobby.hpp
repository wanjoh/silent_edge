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
    ~Lobby();

    bool isEmpty() const;
public slots:
    void onPbExit_clicked();
    void onPbReady_clicked();
    void updateLobby(const QString&);

signals:
    void closeConnection();
    void startGameSignal(const QByteArray);

private:
    Ui::Lobby *ui;
    QString server_ip_;
};


