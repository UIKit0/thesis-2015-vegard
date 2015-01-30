#include "coord.h"

Coord::Coord(GLfloat xc, GLfloat yc)
    : x(xc), y(yc)
{
}

PolarCoord Coord::toPolarCoord() {
    GLfloat r = sqrt(x * x + y * y);
    GLfloat theta = atan2(y, x);
    return PolarCoord(r, theta);
}

PolarCoord::PolarCoord(GLfloat rc, GLfloat thetac)
    : r(rc), theta(thetac)
{
}

Coord PolarCoord::toCoord() {
    GLfloat x = r * cos(theta);
    GLfloat y = r * sin(theta);
    return Coord(x, y);
}
