#include "chessboard.h"

#include <iostream>

#include <QMessageBox>
#include <QDebug>
#include <qpropertyanimation.h>
#include <QApplication>
#include <QPalette>
#include <QPainter>
#include <QFontDatabase>

ChessBoard::ChessBoard(QObject* parent) :
    QGraphicsScene(parent),
    board(),
    engine(0),
	isUpdating(false),
    playerSide(true)
{
    setSceneRect(120, 0, 8 * tileSize, 8 * tileSize);
    //setSceneRect(0, 0, 1000, 600);

    // cần chỉnh sửa lại cái button theo tỉ lệ màn hình

	// 120 + 480 + 400 = 1000
	//sideOfPlayer = addText(((board.sideToMove() == chess::Color::WHITE) ? "White's turn" : "Black's Turn"), QFont("Arial", 20));
    sideOfPlayer = new QGraphicsTextItem("ABC");
    sideOfPlayer->setFont(QFont("Arial", 20, QFont::Bold));
    sideOfPlayer->setDefaultTextColor(Qt::white);
    sideOfPlayer->setPos(0, -(60 - sideOfPlayer->boundingRect().height()/2));
    addItem(sideOfPlayer);

    btnUndo = new SvgButton("./resources/buttons/undo.svg");
    addItem(btnUndo);
	btnUndo->setPos(560, 410);

	btnRedo = new SvgButton("./resources/buttons/redo.svg");
	addItem(btnRedo);
	btnRedo->setPos(620, 410);

	btnLoadGame = new SvgButton("./resources/buttons/loadgame.svg");
	addItem(btnLoadGame);
	btnLoadGame->setPos(690, 410);

	btnSavegame = new SvgButton("./resources/buttons/savegame.svg");
	addItem(btnSavegame);
	btnSavegame->setPos(750, 410);

    btnExit = new ButtonGame("Exit game", QRectF(0, 0, 150, 40));
    addItem(btnExit);
    btnExit->setPos(605, 465);

	btnReverseBoard = new SvgButton("./resources/buttons/reverseboard.svg");
	addItem(btnReverseBoard);
    btnReverseBoard->fixSize(40);
	btnReverseBoard->setPos(485, 485);

	connect(btnReverseBoard, &SvgButton::clicked, this, [&]() {
		reverseBoard = reverseBoard^1;
		syncBoard();
		syncCoordinates();
	});
    
    connect(btnExit, &ButtonGame::clicked, this, [&]() {
        emit exitActivated();
    });
    connect(btnUndo, &SvgButton::clicked, this, [&]() {
        Undo();
    });
    connect(btnRedo, &SvgButton::clicked, this, [&]() {
        Redo();
    });

    for (int i = 0; i < 5; ++i) {
        promotionTiles[i] = new ChessTile(2 + i, 0);
        addItem(promotionTiles[i]);
        promotionTiles[i]->setX(-80);
        promotionTiles[i]->setBrush(Qt::white);

        promotionPieces[i] = new ChessPiece("");
        promotionPieces[i]->setX(-80);
        promotionPieces[i]->setY((2 + i) * tileSize);
        promotionPieces[i]->setZValue(1);
        addItem(promotionPieces[i]);
    }

    for (int i = 0; i < 5; ++i) {
        promotionTiles[i]->hide();
        promotionPieces[i]->hide();
    }

    drawBoard();
    setupMoveLog();
}

void ChessBoard::setTheme(int newTilesTheme, int newPiecesTheme) {
	themePieces = newPiecesTheme;
	themeTiles = newTilesTheme;
}

ChessBoard::~ChessBoard() {
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            delete tiles[i][j];
            delete pieces[i][j];
        }
    }
	delete btnExit;
	delete btnNewGame;
	delete btnUndo;
	delete btnRedo;
	delete selectedPiece;
	delete highlightedTile;
}

void ChessBoard::setGameMode(int mode, int difficulty, int side) {
	gameMode = mode;
	gameDifficulty = difficulty;
    playerSide = side;
	if (gameMode == 1) {
        if (side == 0) reverseBoard = 0;  // player's side is black
		engine.setMode(gameDifficulty);
	}
}

void ChessBoard::drawBoard(void) {
    for (int row = 0; row < boardSize; ++row) {
        for (int col = 0; col < boardSize; ++col) {
            int displayRow = reverseBoard ? row : (boardSize - 1 - row);
            int displayCol = reverseBoard ? col : (boardSize - 1 - col);

            tiles[displayRow][displayCol] = new ChessTile(row, col);
            addItem(tiles[displayRow][displayCol]);

            chess::Square source(chess::Rank(7 - row), chess::File(col));
            chess::Piece piece = board.at<chess::Piece>(source);
            pieces[displayRow][displayCol] = new ChessPiece("");
            pieces[displayRow][displayCol]->setX(displayCol * tileSize);
            pieces[displayRow][displayCol]->setY(displayRow * tileSize);
            if (piece != chess::Piece::NONE) {
                pieces[displayRow][displayCol]->resetPieceImage(themePieces, getPieceIcon(piece));
            }
            addItem(pieces[displayRow][displayCol]);
            pieces[displayRow][displayCol]->setZValue(1);

        }
    }
    for (int row = 0; row < boardSize; ++row) {
        for (int col = 0; col < boardSize; ++col) {
            tiles[row][col]->setTheme(themeTiles);
        }
    }
    sideOfPlayer->setPlainText(((board.sideToMove() == chess::Color::WHITE) ? "White's turn" : "Black's Turn"));
    QFont font("Arial", 10);
    font.setPixelSize(20);
    // Draw row coordinates
    for (int i = 0; i < boardSize; ++i) {
        int File = reverseBoard ? boardSize - i : i + 1;
        txtCoordinatesFile[i] = addText(QString::number(File), font);
        txtCoordinatesFile[i]->setDefaultTextColor(Qt::white);
        txtCoordinatesFile[i]->setPos(-20, i * tileSize + tileSize / 2 - txtCoordinatesFile[i]->boundingRect().height() / 2);
    }
    // Draw column coordinates
    for (int i = 0; i < boardSize; ++i) {
        QChar Rank = reverseBoard ? QChar('a' + i) : QChar('h' - i);
        txtCoordinatesRank[i] = addText(QString(Rank), font);
        txtCoordinatesRank[i]->setDefaultTextColor(Qt::white);
        txtCoordinatesRank[i]->setPos(i * tileSize + tileSize / 2 - txtCoordinatesRank[i]->boundingRect().width() / 2, boardSize * tileSize);
    }
}

void ChessBoard::syncCoordinates() {
    QFont font("Arial", 10);
    font.setPixelSize(20);
    // Draw row coordinates
    for (int i = 0; i < boardSize; ++i) {
        int File = reverseBoard ? boardSize - i : i + 1;
		txtCoordinatesFile[i]->setPlainText(QString::number(File));
        txtCoordinatesFile[i]->setDefaultTextColor(Qt::white);
        txtCoordinatesFile[i]->setPos(-20, i * tileSize + tileSize / 2 - txtCoordinatesFile[i]->boundingRect().height() / 2);
    }
    // Draw column coordinates
    for (int i = 0; i < boardSize; ++i) {
        QChar Rank = reverseBoard ? QChar('a' + i) : QChar('h' - i);
        txtCoordinatesRank[i]->setPlainText(QString(Rank));
        txtCoordinatesRank[i]->setDefaultTextColor(Qt::white);
        txtCoordinatesRank[i]->setPos(i * tileSize + tileSize / 2 - txtCoordinatesRank[i]->boundingRect().width() / 2, boardSize * tileSize);
    }
}

void ChessBoard::syncBoard(void) {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            int displayRow = reverseBoard ? row : (8 - 1 - row);
            int displayCol = reverseBoard ? col : (8 - 1 - col);

            tiles[displayRow][displayCol]->resetTitle();
            chess::Square source(chess::Rank(7 - row), chess::File(col));
            chess::Piece piece = board.at<chess::Piece>(source);
            pieces[displayRow][displayCol]->setX(displayCol * tileSize);
            pieces[displayRow][displayCol]->setY(displayRow * tileSize);
            if (piece != chess::Piece::NONE) {
                pieces[displayRow][displayCol]->resetPieceImage(themePieces, getPieceIcon(piece));
            }
            else {
                pieces[displayRow][displayCol]->resetPieceImage(themePieces, "");
            }
        }
    }
	if (board.getFen() != "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {
		int x = reverseBoard ? 7 - currentMove.from().rank() : (int)currentMove.from().rank();
		int y = reverseBoard ? (int)currentMove.from().file() : 7 - currentMove.from().file();
        tiles[x][y]->toggleHighlight();
		x = reverseBoard ? 7 - currentMove.to().rank() : (int)currentMove.to().rank();
		y = reverseBoard ? (int)currentMove.to().file() : 7 - currentMove.to().file();  
        tiles[x][y]->toggleHighlight();
    }
    if (board.inCheck()) {
		chess::Square king= board.kingSq(board.sideToMove());
		int x = reverseBoard ? 7 - king.rank() : (int)king.rank();
		int y = reverseBoard ? (int)king.file() : 7 - king.file();
		tiles[7 - king.rank()][king.file()]->canAttackHighlight();
    }
    sideOfPlayer->setPlainText(((board.sideToMove() == chess::Color::WHITE) ? "White's turn" : "Black's Turn"));
}

void ChessBoard::syncTiles(void) {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            int displayRow = reverseBoard ? row : (8 - 1 - row);
            int displayCol = reverseBoard ? col : (8 - 1 - col);
            tiles[displayRow][displayCol]->resetTitle();
        }
    }
    if (board.getFen() != "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {
        int x = reverseBoard ? 7 - currentMove.from().rank() : (int)currentMove.from().rank();
        int y = reverseBoard ? (int)currentMove.from().file() : 7 - currentMove.from().file();
        tiles[x][y]->toggleHighlight();
        x = reverseBoard ? 7 - currentMove.to().rank() : (int)currentMove.to().rank();
        y = reverseBoard ? (int)currentMove.to().file() : 7 - currentMove.to().file();
        tiles[x][y]->toggleHighlight();
    }
    if (board.inCheck()) {
        chess::Square king = board.kingSq(board.sideToMove());
        int x = reverseBoard ? 7 - king.rank() : (int)king.rank();
        int y = reverseBoard ? (int)king.file() : 7 - king.file();
        tiles[7 - king.rank()][king.file()]->canAttackHighlight();
    }
}

void ChessBoard::syncPieces(chess::Move nextMove) {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            int displayRow = reverseBoard ? row : (8 - 1 - row);
            int displayCol = reverseBoard ? col : (8 - 1 - col);
            chess::Square source(chess::Rank(7 - row), chess::File(col));
            chess::Piece piece = board.at<chess::Piece>(source);
            if (piece != chess::Piece::NONE) {
                pieces[displayRow][displayCol]->resetPieceImage(themePieces, getPieceIcon(piece));
            }
            else {
                pieces[displayRow][displayCol]->resetPieceImage(themePieces, "");
            }
        }
    }
    sideOfPlayer->setPlainText(((board.sideToMove() == chess::Color::WHITE) ? "White's turn" : "Black's Turn"));
    // Get the starting and ending positions
    //int fromRow = 7 - nextMove.from().rank();
    //int fromCol = nextMove.from().file();
    //int toRow = 7 - nextMove.to().rank();
    //int toCol = nextMove.to().file();
    //chess::Piece piece = board.at<chess::Piece>(nextMove.to());
    //pieces[toRow][toCol]->resetPieceImage(QString("./resources/images/Theme_0/") + getPieceIcon(piece));;
    //pieces[fromRow][fromCol]->resetPieceImage("");
    // Create the animation
  //  QPropertyAnimation* animation = new QPropertyAnimation(pieces[fromRow][fromCol], "pos");
  //  animation->setDuration(150); // Duration of the animation in milliseconds
  //  animation->setStartValue(QPointF(fromCol * tileSize, fromRow * tileSize));
  //  animation->setEndValue(QPointF(toCol * tileSize, toRow * tileSize));
  //  // Connect the animation finished signal to update the piece image
  //  connect(animation, &QPropertyAnimation::finished, [=]() {
  //      chess::Piece piece = board.at<chess::Piece>(nextMove.to());
  //      pieces[toRow][toCol]->resetPieceImage(QString("./resources/images/Theme_0/") + getPieceIcon(piece));;
		//pieces[fromRow][fromCol]->resetPieceImage("");
  //      
  //  });
  //  // Start the animation
  //  animation->start(QAbstractAnimation::DeleteWhenStopped);    
}

void ChessBoard::setSquaresAllowToMove(int row, int col) {
    chess::Square position = chess::Square(chess::Rank(7 - row), chess::File(col));
    chess::Piece piece = board.at(position);
    chess::movegen::legalmoves(legalMoves, board);
    for (const auto& move : legalMoves) {
        if (move.from() == position) {
			chess::Piece targetPiece = board.at(move.to());
			int x = reverseBoard ? 7 - move.to().rank() : (int)move.to().rank();
			int y = reverseBoard ? (int)move.to().file() : 7 - move.to().file();
            if (targetPiece != chess::Piece::NONE) {
				tiles[x][y]->canAttackHighlight();
            }
            else tiles[x][y]->canMoveHighlight();
        }
    }
}

void ChessBoard::unsetSquaresAllowToMove(int row, int col) {
	tiles[row][col]->resetTitle();
    chess::Square position = chess::Square(chess::Rank(7 - row), chess::File(col));
    chess::Piece piece = board.at(position);
    for (const auto& move : legalMoves) {
        if (move.from() == position) {
			tiles[7 - move.to().rank()][move.to().file()]->resetTitle();
        }
    }
}

void ChessBoard::setupPromotion(int oldRow, int oldCol, int row, int col) {
	promotionMode = true;
    if (board.sideToMove() == chess::Color::WHITE) {
        promotionPieces[0]->resetPieceImage(themePieces, "white_queen.svg");
        promotionPieces[1]->resetPieceImage(themePieces, "white_rook.svg");
        promotionPieces[2]->resetPieceImage(themePieces, "white_knight.svg");
        promotionPieces[3]->resetPieceImage(themePieces, "white_bishop.svg");
    }
    else {
        promotionPieces[0]->resetPieceImage(themePieces, "black_queen.svg");
        promotionPieces[1]->resetPieceImage(themePieces, "black_rook.svg");
        promotionPieces[2]->resetPieceImage(themePieces, "black_knight.svg");
        promotionPieces[3]->resetPieceImage(themePieces, "black_bishop.svg");
    }
    promotionPieces[4]->resetPieceImage(themePieces, "cancel-svgrepo-com.svg");
    for (int i = 0; i < 5; ++i) {
        promotionTiles[i]->show();
        promotionPieces[i]->show();
    }
    connect(promotionPieces[0], &ChessPiece::clicked, this, [=]() { promotePawn(oldRow, oldCol, row, col, chess::PieceType::QUEEN); });
    connect(promotionPieces[1], &ChessPiece::clicked, this, [=]() { promotePawn(oldRow, oldCol, row, col, chess::PieceType::ROOK); });
    connect(promotionPieces[2], &ChessPiece::clicked, this, [=]() { promotePawn(oldRow, oldCol, row, col, chess::PieceType::KNIGHT); });
    connect(promotionPieces[3], &ChessPiece::clicked, this, [=]() { promotePawn(oldRow, oldCol, row, col, chess::PieceType::BISHOP); });
    connect(promotionPieces[4], &ChessPiece::clicked, this, [=]() { cancelPromotion(); });
}

void ChessBoard::promotePawn(int oldRow, int oldCol, int row, int col, chess::PieceType promotionPiece) {
    chess::Square from(chess::Rank(7 - oldRow), chess::File(oldCol));
    chess::Square to(chess::Rank(7 - row), chess::File(col));
    chess::Move promotionMove = chess::Move::make<chess::Move::PROMOTION>(from, to, promotionPiece);

    chess::movegen::legalmoves(legalMoves, board);
    bool isLegalMove = false;
    for (const auto& move : legalMoves) {
        if (move == promotionMove) {
            isLegalMove = true;
            break;
        }
    }
    if (isLegalMove) {
        makeUndo(promotionMove);
        resetRedo();
		addMoveToLog();

        board.makeMove(promotionMove);
		syncBoard();
		if (board.inCheck()) soundEffect.soundMoveCheck();
		else soundEffect.soundPromote();
        cancelPromotion();
    }
}

void ChessBoard::cancelPromotion(void) {    
    for (int i = 0; i < 5; ++i) {
        promotionTiles[i]->hide();
        promotionPieces[i]->hide();
		disconnect(promotionPieces[i], nullptr, nullptr, nullptr);
    }
	promotionMode = false;
    syncTiles();
    resetSelecting();
}

bool ChessBoard::playerMove(int row, int col) {
    chess::Square from(chess::Rank(7 - selectedRow), chess::File(selectedCol));
    chess::Square to(chess::Rank(7 - row), chess::File(col));
    bool isLegalMove = false;
	bool isCapture = false; 
    chess::Move nextMove = NULL;
    for (const auto& move : legalMoves) {
        if (move.from() == from and move.to() == to) {
            nextMove = move;
            isLegalMove = true;
			if (board.at<chess::Piece>(to) != chess::PieceType::NONE) isCapture = true;
            break;
        }
    }
    if (isLegalMove) {
        if (board.at<chess::Piece>(from) == chess::PieceType::PAWN && (row == 0 || row == 7)) {
            setupPromotion(selectedRow, selectedCol, row, col);
            
            return false;
        }

        makeUndo(nextMove);
        resetRedo();
        addMoveToLog();
        
        board.makeMove(nextMove);
		syncBoard();

        if (board.inCheck()) soundEffect.soundMoveCheck();
        else if (nextMove.typeOf() == chess::Move::CASTLING) soundEffect.soundCastling();
		else if (isCapture) soundEffect.soundCapture();
        else soundEffect.soundMoveSelf();
        return true;
    }
    return false;
}

void ChessBoard::botMove(void) {
	std::string BotMove = engine.getNextMove(board.getFen());
	chess::Move nextMove = chess::uci::uciToMove(board, BotMove);
	bool isCapture = board.at<chess::Piece>(nextMove.to()) != chess::Piece::NONE;

    makeUndo(nextMove);
    resetRedo();
    addMoveToLog();

	board.makeMove(nextMove);
	syncBoard();
    QTimer::singleShot(500, [this]() {
        if (board.isGameOver().second != chess::GameResult::NONE) {
            drawGameOver();
        }
    });


    if (board.inCheck()) soundEffect.soundMoveCheck();
    else if (nextMove.typeOf() == chess::Move::CASTLING) soundEffect.soundCastling();
    else if (isCapture) soundEffect.soundCapture();
    else soundEffect.soundMoveSelf();
}

void ChessBoard::resetSelecting(void) {
    selectedRow = -1;
    selectedCol = -1;
    if(selectedPiece != nullptr) selectedPiece->setZValue(1);
    selectedPiece = nullptr;
    selecting = false;
    isDragging = false;
}

void ChessBoard::mousePressEvent(QGraphicsSceneMouseEvent* event) {
	if (isUpdating) return;
    isUpdating = true;
    int row = static_cast<int>((event->scenePos().y() / qreal(tileSize)));
    int col = static_cast<int>((event->scenePos().x() / qreal(tileSize)));
    if (event->scenePos().y() < 0) row = -1;
	if (event->scenePos().x() < 0) col = -1;
    int preRow = row, preCol = col;

	row = reverseBoard ? row : 7 - row;
	col = reverseBoard ? col : 7 - col;

     //if(gameMode == 1 && !((playerSide == 1 && board.sideToMove() == chess::Color::WHITE) ||
     //     (playerSide == 0 && board.sideToMove() == chess::Color::BLACK))){
     //     botMove();
     //    isUpdating = false;
     //    return;
     //}

    QPointF scenePos = event->scenePos();

	if (promotionMode) {  // Check if clicked outside promotion area
        bool clickedOutsidePromotionArea = true;
        for (int i = 0; i < 5; ++i) {
            QPointF localPosTile = promotionTiles[i]->mapFromScene(scenePos);
            QPointF localPosPiece = promotionPieces[i]->mapFromScene(scenePos);
            if (promotionTiles[i]->contains(localPosTile) || promotionPieces[i]->contains(localPosPiece)) {
                clickedOutsidePromotionArea = false;
                break;
            }
        }
        if (clickedOutsidePromotionArea) {
            cancelPromotion();
			isUpdating = false;
            QGraphicsScene::mousePressEvent(event);
            return;
        }
    }
    
	if (row < 0 or 8 <= row or col < 0 or 8 <= col) { //outside the board
        if (selecting) {
			unsetSquaresAllowToMove(selectedRow, selectedCol);
            resetSelecting();
        }
        isUpdating = false;
        QGraphicsScene::mousePressEvent(event);
        return;
    }

    chess::Square source(chess::Rank(7 - row), chess::File(col));
    chess::Piece p = board.at<chess::Piece>(source);
    if (!selecting) {
        if (p == chess::Piece::NONE) {
			//resetSelecting();
            isUpdating = false;
            return;
        }
		syncTiles();
        tiles[preRow][preCol]->toggleHighlight();
        setSquaresAllowToMove(row, col);

        selectedRow = row;
        selectedCol = col;      
        selectedPiece = pieces[preRow][preCol];
        selectedPiece->setZValue(2);
        originalPosition = pieces[preRow][preCol]->pos();
        selecting = true;
        isDragging = true;
    }
    else if (selecting) {
        if (row == selectedRow && col == selectedCol) {
			syncTiles();
            resetSelecting();
            isUpdating = false;
            return;
        }
        if (playerMove(row, col)) {
            if (board.isGameOver().second != chess::GameResult::NONE) {
                drawGameOver();
            }
            resetSelecting();
			if (gameMode == 1) {
                QTimer::singleShot(100, [this]() {
                    botMove();
				});
			}
            isUpdating = false;
            return;
        }

		syncTiles();
        if (p != chess::Piece::NONE) {
            tiles[preRow][preCol]->toggleHighlight();
            setSquaresAllowToMove(row, col);

            selectedRow = row;
            selectedCol = col;
            selectedPiece = pieces[preRow][preCol];
            selectedPiece->setZValue(2);
            originalPosition = pieces[preRow][preCol]->pos();
            selecting = true;
            isDragging = true;
        }
        else {
            resetSelecting();
        }
    }
    isUpdating = false;
}

void ChessBoard::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (isDragging) {
        //QPointF newPos = event->scenePos() - QPointF(selectedPiece->boundingRect().width() / 2,
        //    selectedPiece->boundingRect().height() / 2);
        QPointF newPos = event->scenePos() - QPointF(60 / 2,
            60 / 2);
        selectedPiece->setPos(newPos);
		//std::cout << selectedPiece->boundingRect().width() << " " << selectedPiece->boundingRect().height() << std::endl;
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void ChessBoard::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (selecting) {
        int row = static_cast<int>(round(selectedPiece->y() / qreal(tileSize)));
        int col = static_cast<int>(round(selectedPiece->x() / qreal(tileSize)));

		row = reverseBoard ? row : 7 - row;
		col = reverseBoard ? col : 7 - col;

        if (row == selectedRow and col == selectedCol) {
            isDragging = false;
            selectedPiece->setPos(originalPosition);
            QGraphicsScene::mouseReleaseEvent(event);
            return;
        }

        if (0 <= row and row < 8 and 0 <= col and col < 8) {
            if (playerMove(row, col)) {
				if (board.isGameOver().second != chess::GameResult::NONE) {
					drawGameOver();
				}
                resetSelecting();
				if (gameMode == 1) {
                    QTimer::singleShot(100, [this]() {
                        botMove();
                    });
				}
                QGraphicsScene::mouseReleaseEvent(event);
                return;
            }
        }
        selectedPiece->setPos(originalPosition);
        isDragging = false;
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void ChessBoard::drawGameOver(void) {
    QGraphicsRectItem tmp;
    tmp.setRect(-140, -60, 1000, 600);
	tmp.setBrush(QColor(255, 255, 255, 70));
    tmp.setZValue(5);
	addItem(&tmp);

	QMessageBox msgBox;
	msgBox.setText("Game Over");
	msgBox.setInformativeText("Do you want to play again?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	if (ret == QMessageBox::Yes) {
		resetBoard();
	}
	else {
		emit exitActivated();
	}

}

void ChessBoard::Undo(void) {
	if (undoFen.empty()) return;
    chess::Move undoMove = undoFen.top().second;
	redoFen.push(std::make_pair(board.getFen(), currentMove));
	board.setFen(undoFen.top().first);
	undoFen.pop();

	if(!undoFen.empty()) currentMove = undoFen.top().second;
	syncBoard();
    
    if ((gameMode == 1) && (board.sideToMove() != (playerSide ? chess::Color::WHITE : chess::Color::BLACK))) {
        Undo();
    }

    switch (undoMove.typeOf()) {
	    case chess::Move::NORMAL: soundEffect.soundMoveSelf(); break;
        case chess::Move::PROMOTION: soundEffect.soundPromote(); break;
	    case chess::Move::CASTLING: soundEffect.soundCastling(); break;
        case chess::Move::ENPASSANT: soundEffect.soundMoveSelf(); break;
    }

    removeLastMoveFromLog();
}

void ChessBoard::makeUndo(chess::Move nextMove) {
	currentMove = nextMove;
	undoFen.push(std::make_pair(board.getFen(), nextMove));
}

void ChessBoard::Redo(void) {
	if (redoFen.empty()) return;
    currentMove = redoFen.top().second;
	undoFen.push(std::make_pair(board.getFen(), currentMove));
    addMoveToLog();
	board.setFen(redoFen.top().first);
	redoFen.pop();
	
    syncBoard();
    switch (currentMove.typeOf()) {
        case chess::Move::NORMAL: soundEffect.soundMoveSelf(); break;
        case chess::Move::PROMOTION: soundEffect.soundPromote(); break;
        case chess::Move::CASTLING: soundEffect.soundCastling(); break;
        case chess::Move::ENPASSANT: soundEffect.soundMoveSelf(); break;
    }
}

void ChessBoard::resetRedo(void) {
	if (redoFen.empty()) return;
	while (!redoFen.empty()) redoFen.pop();
}

void ChessBoard::resetBoard(void) {
	board = chess::Board();
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			tiles[i][j]->setTheme(themeTiles);  
		}
	}
	//board = chess::Board("8/5Q1p/p5pk/4B3/Pb6/1P3P1P/2q2PK1/3q4 w - - 0 1");
	//std::cout << engine.getNextMove(board.getFen()) << std::endl;
	syncBoard();
	QTimer::singleShot(500, [this]() {
		if (gameMode == 1 && playerSide == 0) {
			botMove();
			syncBoard();
		}
	});
	while (!undoFen.empty()) undoFen.pop();
	while (!redoFen.empty()) redoFen.pop();
}


void ChessBoard::setupMoveLog() {
    moveLog = new QListWidget();
    //moveLog->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //moveLog->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    moveLog->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); 
    moveLog->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    moveLog->setFixedWidth(250); // Adjust the width as needed
    moveLog->setFixedHeight(400);
    
    QFont font = QFont("Courier New", 16);
	font.setBold(true);
    moveLog->setFont(font);


    moveLog->setStyleSheet(
        "QListWidget {"
        "    color: white;"
        "    border: 2px solid green;"
		"    border-radius: 10%;"
		"    background-color: transparent;"
        " margin: 5px;"
        "}"
        "QScrollBar:vertical {"
        "    border: 1px solid #999999;" 
        "    background: #f0f0f0;" 
        "    width: 10px;" 
        "    margin: 0px 0px 0px 0px;" 
        "}" 
        "QScrollBar::handle:vertical {" 
        "    background: #cccccc;" 
        "    min-height: 20px;" 
        "}" 
        "QScrollBar::add-line:vertical {" 
        "    background: #f0f0f0;" 
        "    height: 0px;" 
        "    subcontrol-position: bottom;" 
        "    subcontrol-origin: margin;" 
        "}" 
        "QScrollBar::sub-line:vertical {" 
        "    background: #f0f0f0;" 
        "    height: 0px;" 
        "    subcontrol-position: top;" 
        "    subcontrol-origin: margin;" 
        "}"
    );


    moveLogProxy = addWidget(moveLog);
    moveLogProxy->setPos(555, 0); // Adjust the position as needed
}

void ChessBoard::addMoveToLog(void) {
	QString move = QString(chess::uci::moveToUci(currentMove).c_str());
	int cnt = moveLog->count();
    if (cnt == 0 || moveLog->item(cnt - 1)->text().size() > 9) {
        move = QString::number(board.fullMoveNumber()) + "." + move;
		moveLog->addItem(move);
    }
    else {
		QListWidgetItem* item = moveLog->item(cnt-1);
		item->setText(item->text() + QString(11 - item->text().size(), ' ') + move);
    }
    moveLog->scrollToBottom();
}

void ChessBoard::removeLastMoveFromLog() {
    int cnt = moveLog->count();
	if (cnt == 0) return; 
    QListWidgetItem* item = moveLog->item(cnt - 1);
	int spacePosition = item->text().indexOf(' ');
	if (spacePosition != -1) {
		item->setText(item->text().left(spacePosition));
	}
	else {
		moveLog->takeItem(cnt - 1);
	}
	moveLog->scrollToBottom();
}