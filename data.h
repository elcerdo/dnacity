#ifndef __DATA_HH__
#define __DATA_HH__

#include <QList>
#include <QRectF>

typedef QRectF Rect;
typedef QPointF Point;

// piece of building
struct Piece {
    static Piece random(const Rect& rect);
    Rect base;
    qreal height;
};

typedef QList<Piece> Pieces;

struct Building {
    static Building random(const Rect& rect);
    Pieces pieces;
    Rect bounding;
};

#endif
