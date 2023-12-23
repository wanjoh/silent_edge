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

    //QVariant toVariant() const override;
    //void fromVariant(const QVariant&) override;

private:
    Ui::Lobby *ui;
};


