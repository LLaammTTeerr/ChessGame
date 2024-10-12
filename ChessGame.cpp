#include "ChessGame.h"
#include <QHeaderView>
#include <QHBoxLayout>
#include <QAbstractItemView>
#include <QLabel>

ChessGame::ChessGame(QWidget* parent) :
    QMainWindow(parent),
    chessBoard(new QTableWidget(this)),
    btnNewGame(new QPushButton("New game", this)),
    btnExit(new QPushButton("Exit", this)),
    moveLog(new QListWidget(this))
{
    this->initializeUI();
}

ChessGame::~ChessGame()
{
}

void ChessGame::initializeUI() {
    this->setWindowTitle("Ter Chess");
    this->resize(680, 500);

    QWidget *centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    mainLayout->addLayout(leftLayout, 3);
    
    this->initializeChessBoard();
    leftLayout->addWidget(this->chessBoard);

    QVBoxLayout* rightLayout = new QVBoxLayout();
    mainLayout->addLayout(rightLayout, 1);

    rightLayout->addWidget(btnNewGame);
    rightLayout->addWidget(btnExit);

    rightLayout->addWidget(new QLabel("Move log: ", this));
    rightLayout->addWidget(moveLog);
}

void ChessGame::initializeChessBoard() {
    this->chessBoard->setRowCount(8);
    this->chessBoard->setColumnCount(8);
    this->chessBoard->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->chessBoard->setSelectionMode(QAbstractItemView::NoSelection);
    this->chessBoard->setSelectionBehavior(QAbstractItemView::SelectItems);
    this->chessBoard->setShowGrid(false);
    this->chessBoard->horizontalHeader()->hide();
    this->chessBoard->verticalHeader()->hide();

    for (int i = 0; i < 8; i += 1) {
        this->chessBoard->setRowHeight(i, 60);
        this->chessBoard->setColumnWidth(i, 60);
    }

    for (int row = 0; row < 8; row += 1) {
        for (int col = 0; col < 8; col += 1) {
            QTableWidgetItem* item = new QTableWidgetItem();
            QColor color = (row + col) % 2 ? QColor(0, 0, 0) : QColor(255, 255, 255);
            item->setBackground(color);
            this->chessBoard->setItem(row, col, item);
        }
    }
}
