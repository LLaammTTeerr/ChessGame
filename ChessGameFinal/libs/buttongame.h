#pragma once

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include <QTimer>
#include <QPixmap>

class ButtonGame : public QObject, public QGraphicsRectItem {
    Q_OBJECT

public:
    ButtonGame(const QString& text, const QRectF& rect, QGraphicsItem* parent = nullptr);
    void setBackgroundImage(const QString& imagePath); // Thêm phương thức để thiết lập hình ảnh background
    void setHoverImage(const QString& imagePath);
    void setClickyImage(const QString& imagePath);
	void resetBackground(void);

signals:
    void clicked();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

private:
    QPixmap backgroundImage, hoverImage, clickyImage;
};

