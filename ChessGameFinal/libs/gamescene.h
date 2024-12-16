#pragma once

#include <QApplication>
#include <QGraphicsView>
#include <QBrush>
#include <QColor>
#include <QGraphicsSceneMouseEvent>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QStackedWidget>

class GameScene : public QWidget {
    Q_OBJECT

public:
    GameScene(QWidget* parent = nullptr);
    ~GameScene();
};

