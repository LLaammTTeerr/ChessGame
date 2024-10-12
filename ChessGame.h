#pragma once

#include <QtWidgets/QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QListWidget>

class ChessGame : public QMainWindow
{
    Q_OBJECT

public:
    ChessGame(QWidget *parent = nullptr);
    ~ChessGame();

private:
    QTableWidget *chessBoard;
    QPushButton *btnNewGame, *btnExit;
    QListWidget *moveLog;

    void initializeUI();
    void initializeChessBoard();
};
