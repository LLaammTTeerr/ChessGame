#pragma once

#include "chess.hpp"
#include <QString>
#include <string>

const QString _piecesOrder[] = {
	"pawn",
	"knight",
	"bishop",
	"rook",
	"queen",
	"king"
};

inline QString getPieceColor(chess::Piece piece) {
	return (piece.color() == chess::Color::WHITE) ? "white" : "black";
}

inline QString getPieceType(chess::Piece piece) {
	return _piecesOrder[piece.type()];
}

inline QString getPieceIcon(chess::Piece piece) {
	return getPieceColor(piece) + '_' + getPieceType(piece) + ".svg";
}