#pragma once

#include <QtWidgets/QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QListWidget>
#include "chess.hpp"
#include "ChessBoard.h"

class ChessGame : public QMainWindow
{
    Q_OBJECT

public:
    ChessGame(QWidget *parent = nullptr);
    ~ChessGame();

private:
    ChessBoard *chessBoard;
    QPushButton *btnNewGame, *btnExit;
    QListWidget *moveLog;

    void initializeUI(void);
    void onExitClicked(void);
    void onNewGameClicked(void);
};
