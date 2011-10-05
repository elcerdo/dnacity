#include "data.h"

#include <cstdlib>

qreal randFlat(qreal min, qreal max)
{
    const qreal alpha = static_cast<qreal>(qrand())/RAND_MAX;
    return min + alpha*(max-min);
}

Point randPoint(const Rect& rect)
{
    return Point(randFlat(rect.left(),rect.right()),randFlat(rect.top(),rect.bottom()));
}

Rect randRect(const Rect& rect)
{
    const Point p0 = randPoint(rect);
    const Point p1 = randPoint(rect);
    const Point topLeft(qMin(p0.x(),p1.x()),qMin(p0.y(),p1.y()));
    const Point bottomRight(qMax(p0.x(),p1.x()),qMax(p0.y(),p1.y()));
    return Rect(topLeft,bottomRight);
}

Piece Piece::random(const Rect& rect)
{
    Piece piece;
    piece.height = randFlat(.3,.6);
    piece.base = randRect(rect);
    return piece;
}

Building Building::random(const Rect& rect)
{
    Building building;
    for (int kk=0; kk<3; kk++) building.pieces.append(Piece::random(rect));
    return building;
}

