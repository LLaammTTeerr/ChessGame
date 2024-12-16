#include "view.h"

View::View() :
	m_gameScene(new GameScene()),
	m_chessBoard(new ChessBoard()),
	m_mainMenu(new MainMenu()),
	m_themesSetting(new ThemesSetting()),
	m_settingsMenu(new SettingsMenu())
{
	setWindowTitle("Chess Game BL");
	setFixedSize(QSize(Width, Height));
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	setStyleSheet(QString("QGraphicsView { border-image: url(%1) 0 0 0 0 stretch stretch; }").arg("./resources/backgroundimage/chessgame_menu.jpg"));
	

	setScene(m_mainMenu);
	//setScene(m_settingsMenu);

	// Cursor style
	QPixmap pixmap("./resources/cursor/cursor.png");
	customCursor = QCursor(pixmap);
	//setCursor(customCursor);

	// Background ChessBoard
	//setScene(m_mainMenu);
	//setStyleSheet("background-image: url(./resources/backgroundimage/chessboard_background.jpg); background-repeat: no-repeat; background-position: center;");

	
	// Main Menu
	connect(m_mainMenu, &MainMenu::playgameActivated, [this]() {
		setStyleSheet(QString("QGraphicsView { border-image: url(%1) 0 0 0 0 stretch stretch; }").arg("./resources/backgroundimage/chessboard_background.jpg"));
		m_chessBoard->setGameMode(m_mainMenu->gameMode, m_mainMenu->gameDifficulty, m_mainMenu->playerSide);
		m_chessBoard->resetBoard();
		setScene(m_chessBoard);
	});
	connect(m_mainMenu, &MainMenu::themeActivated, [this]() {
		setScene(m_themesSetting);
	});
	connect(m_mainMenu, &MainMenu::settingsActivated, [this]() {
		setScene(m_settingsMenu);
	});
	connect(m_mainMenu, &MainMenu::exitActivated, [this]() {
		close();
	});

	// Themes Setting
	connect(m_themesSetting, &ThemesSetting::backActivated, [this]() {
		setStyleSheet(QString("QGraphicsView { border-image: url(%1) 0 0 0 0 stretch stretch; }").arg("./resources/backgroundimage/chessgame_menu.jpg"));
		setScene(m_mainMenu);
	});
	connect(m_themesSetting, &ThemesSetting::saveChangeActivated, [this]() {
		setStyleSheet(QString("QGraphicsView { border-image: url(%1) 0 0 0 0 stretch stretch; }").arg("./resources/backgroundimage/chessgame_menu.jpg"));
		m_chessBoard->setTheme(m_themesSetting->tilesTheme, m_themesSetting->piecesTheme);
	});

	// Settings Menu
	connect(m_settingsMenu, &SettingsMenu::backActivated, [this]() {
		setStyleSheet(QString("QGraphicsView { border-image: url(%1) 0 0 0 0 stretch stretch; }").arg("./resources/backgroundimage/chessgame_menu.jpg"));
		setScene(m_mainMenu);
	});

	// ChessBoard
	connect(m_chessBoard, &ChessBoard::menuActivated, [this]() {
		setStyleSheet(QString("QGraphicsView { border-image: url(%1) 0 0 0 0 stretch stretch; }").arg("./resources/backgroundimage/chessgame_menu.jpg"));
		setScene(m_mainMenu);
	});
	connect(m_chessBoard, &ChessBoard::exitActivated, [this]() {
		close();
	});
}

void View::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Escape) {
		QApplication::quit();
	}
	else {
		QGraphicsView::keyPressEvent(event);
	}
}

//void View::mousePressEvent(QMouseEvent* event) {
//	setCursor(Qt::ClosedHandCursor);
//	QGraphicsView::mousePressEvent(event);
//}
//
//
//void View::mouseReleaseEvent(QMouseEvent* event) {
//	setCursor(customCursor);
//	QGraphicsView::mouseReleaseEvent(event);
//}