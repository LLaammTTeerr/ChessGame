#include "ChessGame.h"
#include <QHeaderView>
#include <QHBoxLayout>
#include <QAbstractItemView>
#include <QLabel>
#include <QCoreApplication>
#include <QMessageBox>

ChessGame::ChessGame(QWidget* parent) :
    QMainWindow(parent),
    chessBoard(new ChessBoard(this)),
    btnNewGame(new QPushButton("New game", this)),
    btnExit(new QPushButton("Exit", this)),
    moveLog(new QListWidget(this))
{
    this->initializeUI();
}

ChessGame::~ChessGame(void) {
}

void ChessGame::initializeUI(void) {
    this->setWindowTitle("Ter Chess");
    this->setFixedSize(680, 500);

    QWidget *centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    mainLayout->addLayout(leftLayout, 3);
    
    leftLayout->addWidget(this->chessBoard);

    QVBoxLayout* rightLayout = new QVBoxLayout();
    mainLayout->addLayout(rightLayout, 1);

    rightLayout->addWidget(btnNewGame);
    rightLayout->addWidget(btnExit);

    rightLayout->addWidget(new QLabel("Move log: ", this));
    rightLayout->addWidget(moveLog);

    connect(btnExit, &QPushButton::clicked, this, &ChessGame::onExitClicked);
}

void ChessGame::onExitClicked(void) {
    close();
}