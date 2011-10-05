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
    return Rect(randPoint(rect),randPoint(rect));
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

