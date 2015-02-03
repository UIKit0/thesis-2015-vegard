#include "point.h"

Point::Point(GLfloat xc, GLfloat yc)
    : x(xc), y(yc)
{
}

/**
 * Convert to polar coordinates.
 */
Polar Point::toPolar() const {
    GLfloat r = sqrt(x * x + y * y);
    GLfloat theta = atan2(y, x);
    return Polar(r, theta);
}

Polar::Polar(GLfloat rc, GLfloat thetac)
    : r(rc), theta(thetac)
{
}

/**
 * Convert to cartesian coordinates.
 */
Point Polar::toPoint() const {
    GLfloat x = r * cos(theta);
    GLfloat y = r * sin(theta);
    return Point(x, y);
}
