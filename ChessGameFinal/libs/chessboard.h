#pragma once

#include "chesstile.h"
#include "chesspiece.h"
#include "buttongame.h"
#include "utilities.h"
#include "chess.hpp"
#include "StockfishHandler.h"
#include "soundeffects.h"
#include "svgbutton.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QListWidget>
#include <QGraphicsProxyWidget>
#include <stack>

class GameScene;

class ChessBoard : public QGraphicsScene {
	Q_OBJECT

public:
	ChessBoard(QObject *parent = nullptr);
	~ChessBoard();
	void setGameMode(int, int, int);
	void resetBoard(void);
	int themePieces = 0, themeTiles = 0;


signals:
	void menuActivated();
	void exitActivated();

public slots:
	void setTheme(int, int);
	//void loadGame(void);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
	int playerSide = 1;   // 1: white, 0: black
	int reverseBoard = 1;
	bool isUpdating;
	int gameMode; // 0: PvP, 1: PvE
	int gameDifficulty; // 0: Easy, 1: Medium, 2: Hard

	chess::Color player1Side = chess::Color::WHITE, player2Side = chess::Color::BLACK;
	chess::Board board;
	chess::Movelist legalMoves;
	chess::Move currentMove;
	
	StockFish engine;
	SoundEffect soundEffect;
		
	ChessTile* tiles[boardSize][boardSize];
	ChessPiece* pieces[boardSize][boardSize];
	QGraphicsTextItem* txtCoordinatesRank[8], * txtCoordinatesFile[8];

	ChessPiece* selectedPiece = nullptr;
	QPointF originalPosition;
	ChessTile* highlightedTile = nullptr;  // Lưu ô đã highlight
	int selectedRow, selectedCol;
	bool selecting = false, isDragging = false;

	QGraphicsTextItem *sideOfPlayer;
	ButtonGame *btnExit, *btnNewGame; //*btnUndo, *btnRedo;
	SvgButton *btnReverseBoard;
	SvgButton *btnUndo, *btnRedo, *btnLoadGame, *btnSavegame;

	void drawBoard(void);
	void syncCoordinates();
	void syncBoard(void);
	void syncTiles(void);
	void syncPieces(chess::Move);
	void setSquaresAllowToMove(int, int);
	void unsetSquaresAllowToMove(int, int);

	bool promotionMode = false;
	ChessTile* promotionTiles[5];
	ChessPiece* promotionPieces[5];
	void setupPromotion(int, int, int, int);
	void promotePawn(int, int, int, int, chess::PieceType);
	void cancelPromotion(void);

	bool playerMove(int, int);
	void botMove(void);
	void resetSelecting(void);

	void drawGameOver(void);
	
	std::stack<std::pair<std::string, chess::Move>> undoFen, redoFen;
	void Undo(void);
	void makeUndo(chess::Move);
	void Redo(void);
	void resetRedo(void);

	// Move log related methods and variables
	QListWidget* moveLog;
	QGraphicsProxyWidget* moveLogProxy;
	void addMoveToLog(void);
	void removeLastMoveFromLog();
	void setupMoveLog();
};

