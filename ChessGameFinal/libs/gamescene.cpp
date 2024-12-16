#include "gamescene.h"

GameScene::GameScene(QWidget* parent) :
    QWidget(parent) 
{
    //QVBoxLayout* mainLayout = new QVBoxLayout(this);
    //QStackedWidget* stackedWidget = new QStackedWidget(this);

    //// Cảnh 1: Bàn cờ và nút "New Game"
    //QWidget* chessSceneWidget = new QWidget();
    //QHBoxLayout* chessLayout = new QHBoxLayout(chessSceneWidget);

    //QGraphicsView* view = new QGraphicsView();
    //ChessBoard* board = new ChessBoard();
    //view->setScene(board);
    //view->setFixedSize(1296, 729);

    //QPushButton* newGameButton = new QPushButton("New Game");
    //connect(newGameButton, &QPushButton::clicked, [stackedWidget]() {
    //    stackedWidget->setCurrentIndex(1);  // Chuyển sang cảnh mới
    //    });

    //chessLayout->addWidget(view);
    //chessLayout->addWidget(newGameButton);

    //// Cảnh 2: Hiển thị nút "Hello World"
    //QWidget* helloSceneWidget = new QWidget();
    //QVBoxLayout* helloLayout = new QVBoxLayout(helloSceneWidget);

    //QPushButton* helloButton = new QPushButton("Hello World");
    //helloLayout->addWidget(helloButton);
    //helloLayout->setAlignment(Qt::AlignCenter);

    //// Kết nối nút "Hello World" để chuyển về cảnh 1
    //connect(helloButton, &QPushButton::clicked, [stackedWidget]() {
    //    stackedWidget->setCurrentIndex(0);  // Quay lại cảnh bàn cờ
    //    });

    //// Thêm cả hai cảnh vào QStackedWidget

    ////chessSceneWidget->setStyleSheet("background: transparent; ");

    //stackedWidget->addWidget(chessSceneWidget);  // Index 0: Cảnh bàn cờ
    //stackedWidget->addWidget(helloSceneWidget);  // Index 1: Cảnh Hello World

    //mainLayout->addWidget(stackedWidget);
}

GameScene::~GameScene() {

}
