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
    btnExit(new QPushButton("Exit", this))
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
    rightLayout->addWidget(chessBoard->moveLog);

    connect(btnExit, &QPushButton::clicked, this, &ChessGame::onExitClicked);
    connect(btnNewGame, &QPushButton::clicked, this, &ChessGame::onNewGameClicked);
}

void ChessGame::onExitClicked(void) {
    close();
}

void ChessGame::onNewGameClicked(void) {
    QMessageBox modeGameQuery(this);
    QPushButton* PvPButton = modeGameQuery.addButton(QString("PvP"), QMessageBox::AcceptRole);
    QPushButton* PvEutton = modeGameQuery.addButton(QString("PvE"), QMessageBox::AcceptRole);
    QPushButton* cancelButton = modeGameQuery.addButton(QMessageBox::Cancel);
    modeGameQuery.setDefaultButton(PvPButton);
    modeGameQuery.setWindowTitle("Game mode");
    modeGameQuery.setText("Choose your game mode!");
    modeGameQuery.exec();

    if (modeGameQuery.clickedButton() == cancelButton) {
        return;
    }

    bool PvP = modeGameQuery.clickedButton() == PvPButton;

    chessBoard->reset(PvP);
}