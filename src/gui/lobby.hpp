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
    explicit Lobby(QWidget *parent = nullptr);
    ~Lobby();

    bool isEmpty() const;
public slots:
    void onPbExit_clicked();

signals:
    void closeConnection();

private:
    Ui::Lobby *ui;
};


