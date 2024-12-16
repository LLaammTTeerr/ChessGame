#include "buttongame.h"
#include <QBrush>
#include <QPen>
#include <QFont>

ButtonGame::ButtonGame(const QString& text, const QRectF& rect, QGraphicsItem* parent)
    : QGraphicsRectItem(rect, parent) {
    setAcceptHoverEvents(true);
    setPen(Qt::NoPen);
    // Thêm văn bản vào button
    QGraphicsTextItem* label = new QGraphicsTextItem(text, this);
	label->setFont(QFont("Courier New", 14));
	label->setDefaultTextColor(Qt::white);
    label->setPos(rect.width() / 2 - label->boundingRect().width() / 2,
                  rect.height() / 2 - label->boundingRect().height() / 2); // Căn giữa văn bản
    setBackgroundImage("./resources/buttons/button_1.png");
    setHoverImage("./resources/buttons/button_1_hover.png");
    setClickyImage("./resources/buttons/button_1_clicky.png");
}

void ButtonGame::resetBackground(void) {
	setBrush(QBrush(backgroundImage.scaled(rect().size().toSize(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
}

/*....................Set Image Background....................*/
void ButtonGame::setBackgroundImage(const QString& imagePath) {
    backgroundImage.load(imagePath); // Tải hình ảnh
    setBrush(QBrush(backgroundImage.scaled(rect().size().toSize(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation))); // Thiết lập hình ảnh làm background
}

void ButtonGame::setHoverImage(const QString& imagePath) {
    hoverImage.load(imagePath);
}

void ButtonGame::setClickyImage(const QString& imagePath) {
    clickyImage.load(imagePath);
}
///////////////////////////////////////////////////////////////


/*....................Mouse Event....................*/
void ButtonGame::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    emit clicked();
    //setBrush(QBrush(clickyImage.scaled(rect().size().toSize(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    //QTimer::singleShot(50, [this]() {
    //    setBrush(QBrush(hoverImage.scaled(rect().size().toSize(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    //});
    //event->accept();
	setScale(1.0);
    QGraphicsRectItem::mousePressEvent(event);
}

void ButtonGame::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {
    //setBrush(QBrush(hoverImage.scaled(rect().size().toSize(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    setScale(1.05);
    QGraphicsRectItem::hoverEnterEvent(event);
}

void ButtonGame::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
    //setBrush(QBrush(backgroundImage.scaled(rect().size().toSize(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    setScale(1.0);
    QGraphicsRectItem::hoverLeaveEvent(event);
}
//////////////////////////////////////////////////////