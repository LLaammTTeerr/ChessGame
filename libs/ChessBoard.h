#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <QPushButton>
#include <vector>
#include "libs/chess.hpp"
#include <QListWidget>
#include "StockfishHandler.h"

class ChessBoard : public QWidget
{
    Q_OBJECT

public:
    explicit ChessBoard(QWidget* parent = nullptr);
    ~ChessBoard();

    QListWidget* moveLog;

    QPushButton* at(int, int);
    void reset(bool);
private:
    StockFish engine;
    chess::Board board;
    int selectedRow, selectedCol;
    bool selecting;
    bool PvP;

    std::vector<std::vector<QPushButton*>> squares;
    void setupBoard(void);
    void syncBoard(void);
    void resetCellColor(int, int);
    void onCellClicked(int, int);
};

#endif // CHESSBOARD_H
