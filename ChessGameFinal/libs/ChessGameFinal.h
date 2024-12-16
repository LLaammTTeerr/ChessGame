#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ChessGameFinal.h"

class ChessGameFinal : public QMainWindow
{
    Q_OBJECT

public:
    ChessGameFinal(QWidget *parent = nullptr);
    ~ChessGameFinal();

private:
    Ui::ChessGameFinalClass ui;
};
