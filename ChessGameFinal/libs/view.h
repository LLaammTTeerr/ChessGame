#pragma once

#include "utilities.h"
#include "gamescene.h"
#include "chessboard.h"
#include "mainmenu.h"
#include "themesSetting.h"
#include "settingsMenu.h"
#include <QGraphicsView>
#include <QKeyEvent>


//namespace SCREEN
//{
//	static constexpr const QSize LOGICAL_SIZE = QSize(1300, 730);
//	static constexpr const QSize CELL_SIZE = QSize(1, 1);
//	static constexpr const QSize PHYSICAL_SIZE = QSize(LOGICAL_SIZE.width() * CELL_SIZE.width(),
//		LOGICAL_SIZE.height() * CELL_SIZE.height());
//};


class View : public QGraphicsView
{
	Q_OBJECT
public:
	explicit View();
	static constexpr const int Width = 1000;
	static constexpr const int Height = 600;


protected:
	void keyPressEvent(QKeyEvent* event) override;
	//void mousePressEvent(QMouseEvent* event) override;
	//void mouseReleaseEvent(QMouseEvent* event) override;

private:
	GameScene* m_gameScene;
	ChessBoard* m_chessBoard;
	MainMenu* m_mainMenu;
	ThemesSetting* m_themesSetting;
	SettingsMenu* m_settingsMenu;

	QCursor customCursor;
};

