#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <QPushButton>
#include <vector>
#include "libs/chess.hpp"

class ChessBoard : public QWidget
{
    Q_OBJECT

public:
    explicit ChessBoard(QWidget* parent = nullptr);
    ~ChessBoard();

    QPushButton* at(int, int);
private:
    chess::Board board;
    int selectedRow, selectedCol;
    bool selecting;

    std::vector<std::vector<QPushButton*>> squares;
    void setupBoard(void);
    void syncBoard(void);
    void resetCellColor(int, int);
    void onCellClicked(int, int);
};

#endif // CHESSBOARD_H
