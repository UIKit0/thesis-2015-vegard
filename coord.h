#include <QGLWidget>
#include <cmath>

class PolarCoord;

/**
 * Cartesian coordinates.
 */
class Coord
{
public:
    Coord(GLfloat xc, GLfloat yc);
    PolarCoord toPolarCoord() const;

    GLfloat x;
    GLfloat y;
};

/**
 * Polar coordinates.
 */
class PolarCoord
{
public:
    PolarCoord(GLfloat rc, GLfloat thetac);
    Coord toCoord() const;

    GLfloat r;
    GLfloat theta;
};
