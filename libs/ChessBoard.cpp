#include "ChessBoard.h"
#include <QGridLayout>
#include <QIcon>
#include <QDebug>
#include "utilities.h"
#include <QMessageBox>

ChessBoard::ChessBoard(QWidget* parent) : QWidget(parent), board()
{
    setupBoard();
}

ChessBoard::~ChessBoard(void)
{
}

void ChessBoard::resetCellColor(int row, int col) {
    QPushButton* button = this->at(row, col);
    if ((row + col) % 2 == 0) {
        button->setStyleSheet("background-color: white; border: none");
    }
    else {
        button->setStyleSheet("background-color: gray; border: none");
    }
}

void ChessBoard::setupBoard(void)
{
    QGridLayout* gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(0);

    squares.resize(8, std::vector<QPushButton*>(8, nullptr));

    for (int row = 0; row < 8; row += 1) {
        for (int col = 0; col < 8; col += 1) {
            QPushButton* button = new QPushButton(this);
            button->setFixedSize(60, 60);
            button->setFlat(true);
            button->setIconSize(QSize(50, 50));
            if ((row + col) % 2 == 0) {
                button->setStyleSheet("background-color: white; border: none");
            }
            else {
                button->setStyleSheet("background-color: gray; border: none");
            }
            gridLayout->addWidget(button, row, col);
            squares[row][col] = button;
            connect(button, &QPushButton::clicked, this, [&, row, col](void) {
                onCellClicked(row, col);
            });
        }
    }

    setLayout(gridLayout);
    syncBoard();
}

void ChessBoard::onCellClicked(int row, int col) {
    QPushButton* button = this->at(row, col);
    if (not selecting) {
        chess::Square position = chess::Square(chess::Rank(7 - row), chess::File(col));
        chess::Piece piece = board.at(position);
        chess::Movelist legalMoves;
        chess::movegen::legalmoves(legalMoves, board);
        for (const auto& move : legalMoves) {
            if (move.from() == position) {
                this->at(7 - move.to().rank(), move.to().file())->setStyleSheet("background-color: green; border: none");
            }
        }
        if (piece != chess::Piece::NONE and board.sideToMove() == piece.color()) {
            selectedRow = row;
            selectedCol = col;
            selecting = true;
            button->setStyleSheet("background-color: yellow; border: none");
        }
    } else {
        chess::Square from(chess::Rank(7 - selectedRow), chess::File(selectedCol));
        chess::Square to(chess::Rank(7 - row), chess::File(col));
        chess::Movelist legalMoves;
        chess::movegen::legalmoves(legalMoves, board);
        bool isLegalMove = false;
        chess::Move nextMove = NULL;
        for (const auto& move : legalMoves) {
            if (move.from() == from and move.to() == to) {
                nextMove = move;
                isLegalMove = true;
            }
            if (move.from() == from) {
                resetCellColor(7 - move.to().rank(), move.to().file());
            }
        }
        if (from != to and isLegalMove) {
            board.makeMove(nextMove);
            syncBoard();
        }
        resetCellColor(row, col);
        resetCellColor(selectedRow, selectedCol);
        selectedRow = selectedCol = -1;
        selecting = false;
        auto [resultReason, result] = board.isGameOver();
        if (result == chess::GameResult::NONE) {
            return;
        }
        QMessageBox resultMessage;
        resultMessage.setText("Game End");
        resultMessage.exec();
    }
}

QPushButton* ChessBoard::at(int row, int col) {
    return this->squares[row][col];
}

void ChessBoard::syncBoard(void) {
    for (int row = 0; row < 8; row += 1) {
        for (int col = 0; col < 8; col += 1) {
            chess::Square source(chess::Rank(7 - row), chess::File(col));
            chess::Piece piece = board.at<chess::Piece>(source);
            this->at(row, col)->setIcon(QIcon());
            if (piece != chess::Piece::NONE) {
                QIcon icon = QIcon(QString("./resources/images/theme_2/") + getPieceIcon(piece));
                this->at(row, col)->setIcon(icon);
            }
        }
    }
}

void ChessBoard::reset(void) {
    board = chess::Board();
    selectedRow = selectedCol = -1;
    selecting = false;
    syncBoard();
}