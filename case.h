#include <QtOpenGL>

#include "grid.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

/**
 * Case class.
 *
 * This class uses the Template Method pattern from the
 * Design Patterns book (Gang of Four, 1995). That is,
 * parts of the case code are redefined in subclasses.
 */
class Case
{
public:
    Case(const int n, const char *str);

    virtual void initialize();
    virtual void initializeImage();
    virtual void initializeGrid();
    virtual void initializeProgram();
    virtual void initializeGL();
    virtual void paint();
    virtual void run();
    virtual void addTime(int time);
    virtual float averageTime();
    virtual void printAverage();

    int id;
    QString title;

protected:
    void createProgram(const char *vshader, const char *fshader);
    GLuint loadShaderFromResource(GLenum type, QString resource);
    GLuint loadShader(GLenum type, const char *shaderSrc);
    void byteSwapImage(QImage &img, GLenum pixel_type);

    Grid grid;
    GLuint program;
    QImage image;
    bool initialized;
    QList<int> times;
};

class Case1 : public Case
{
public:
    Case1() : Case(1, "Forward mapping on the CPU") { }
    void initializeGrid();
};

class Case2 : public Case
{
public:
    Case2() : Case(2, "Backward mapping on the CPU") { }
    void initializeGrid();
};

class Case3 : public Case
{
public:
    Case3() : Case(3, "Forward mapping on the GPU") { }
    void initializeProgram();
};

class Case4 : public Case
{
public:
    Case4() : Case(4, "Backward mapping on the GPU") { }
    void initializeProgram();
};
