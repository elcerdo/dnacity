#ifndef __DATA_HH__
#define __DATA_HH__

#include <QList>
#include <QRectF>

typedef QRectF Rect;
typedef QPointF Point;

// piece of building
struct Piece {
    static Piece random(const Rect& rect, qreal minHeight, qreal maxHeight);
    Rect base;
    qreal height;
};

typedef QList<Piece> Pieces;

struct Building {
    static Building random(const Rect& rect, int minPieces=3, int maxPieces=6, qreal minHeight=0.3, qreal maxHeight=0.7);
    Pieces pieces;
    Rect bounding;
};

#endif
