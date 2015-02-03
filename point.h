#include <QGLWidget>
#include <cmath>

class Polar;

/**
 * Cartesian coordinates.
 */
class Point
{
public:
    Point(GLfloat xc, GLfloat yc);
    Polar toPolar() const;

    GLfloat x;
    GLfloat y;
};

/**
 * Polar coordinates.
 */
class Polar
{
public:
    Polar(GLfloat rc, GLfloat thetac);
    Point toPoint() const;

    GLfloat r;
    GLfloat theta;
};
