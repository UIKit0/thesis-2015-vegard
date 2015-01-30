#include <QGLWidget>
#include <cmath>

class PolarCoord;

class Coord
{
public:
    Coord(GLfloat xc, GLfloat yc);
    PolarCoord toPolarCoord();

    GLfloat x;
    GLfloat y;
};

class PolarCoord
{
public:
    PolarCoord(GLfloat rc, GLfloat thetac);
    Coord toCoord();

    GLfloat r;
    GLfloat theta;
};
