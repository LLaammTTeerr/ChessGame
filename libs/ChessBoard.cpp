#include "ChessBoard.h"
#include <QGridLayout>
#include <QIcon>
#include <QDebug>
#include "utilities.h"
#include <QMessageBox>

ChessBoard::ChessBoard(QWidget* parent) : QWidget(parent), moveLog(new QListWidget(this)), board(), PvP(true), engine(0)
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


void ChessBoard::makeSquaresAllowToMove(int row, int col) {
    QPushButton* button = this->at(row, col);
    
    chess::Square position = chess::Square(chess::Rank(7 - row), chess::File(col));
    chess::Piece piece = board.at(position);
    if (piece == chess::Piece::NONE or board.sideToMove() != piece.color()) return;

    chess::Movelist legalMoves;
    chess::movegen::legalmoves(legalMoves, board);
    for (const auto& move : legalMoves) {
        if (move.from() == position) {
            this->at(7 - move.to().rank(), move.to().file())->setStyleSheet("background-color: green; border: none");
        }
    }
    selectedRow = row;
    selectedCol = col;
    selecting = true;
    button->setStyleSheet("background-color: yellow; border: none");
}

void ChessBoard::playerMove(int row, int col) {
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
        moveLog->addItem(QString(chess::uci::moveToLan(board, nextMove).c_str()));
        board.makeMove(nextMove);
        syncBoard();
    }
    resetCellColor(row, col);
    resetCellColor(selectedRow, selectedCol);
    selectedRow = selectedCol = -1;
    selecting = false;
}

void ChessBoard::botMove(void) {
    if (not PvP && board.sideToMove() == chess::Color::BLACK) {
        std::string BotMove = engine.getNextMove(board.getFen());
        moveLog->addItem(QString(BotMove.c_str()));
        board.makeMove(chess::uci::uciToMove(board, BotMove));
        syncBoard();
    }
}

void ChessBoard::onCellClicked(int row, int col) {
    QPushButton* button = this->at(row, col);
    if (not PvP and board.sideToMove() != chess::Color::WHITE) {
        return;
    }
    if (not selecting) {
        makeSquaresAllowToMove(row, col);
    } 
    else {
        playerMove(row, col);
        if (checkGameEnd()) return;

        botMove();        
        if (checkGameEnd()) return;
    }
}

bool ChessBoard::checkGameEnd(void) {
    auto [resultReason, result] = board.isGameOver();
    if (result == chess::GameResult::NONE) {
        return false;
    }
    QMessageBox resultMessage;
    resultMessage.setText("Game End");
    resultMessage.exec();
    return true;
}

void ChessBoard::Undo(void) {

}

void ChessBoard::Redo(void) {

}


// OK
QPushButton* ChessBoard::at(int row, int col) {
    return this->squares[row][col];
}

// OK
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

// OK
void ChessBoard::reset(bool PvP) {
    board = chess::Board();
    selectedRow = selectedCol = -1;
    selecting = false;
    for (int row = 0; row < 8; row += 1) {
        for (int col = 0; col < 8; col += 1) {
            QPushButton* button = squares[row][col];
            if ((row + col) % 2 == 0) {
                button->setStyleSheet("background-color: white; border: none");
            }
            else {
                button->setStyleSheet("background-color: gray; border: none");
            }
        }
    }
    this->moveLog->clear();
    this->PvP = PvP;
    syncBoard();
}