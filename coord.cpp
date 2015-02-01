#include "coord.h"

Coord::Coord(GLfloat xc, GLfloat yc)
    : x(xc), y(yc)
{
}

/**
 * Convert to polar coordinates.
 */
PolarCoord Coord::toPolarCoord() const {
    GLfloat r = sqrt(x * x + y * y);
    GLfloat theta = atan2(y, x);
    return PolarCoord(r, theta);
}

PolarCoord::PolarCoord(GLfloat rc, GLfloat thetac)
    : r(rc), theta(thetac)
{
}

/**
 * Convert to cartesian coordinates.
 */
Coord PolarCoord::toCoord() const {
    GLfloat x = r * cos(theta);
    GLfloat y = r * sin(theta);
    return Coord(x, y);
}
