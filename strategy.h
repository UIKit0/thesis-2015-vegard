#include <QtOpenGL>

#include "grid.h"
#include "measurements.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

/**
 * Strategy class.
 *
 * This class uses the Template Method pattern from the
 * Design Patterns book (Gang of Four, 1995). That is,
 * parts of the case code are redefined in subclasses.
 */
class Strategy
{
public:
    Strategy(const int n, const char *str);

    virtual void initialize();
    virtual void initializeImage();
    virtual void initializeGrid();
    virtual void initializeProgram();
    virtual void initializeGL();
    virtual void paint();
    virtual void run();
    virtual void printMeasurements();

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
    Measurements measurements;
};

class StrategyCPFB : public Strategy
{
public:
    StrategyCPFB() : Strategy(1, "CPFB") { }
    void initializeGrid();
};

class StrategyCPBB : public Strategy
{
public:
    StrategyCPBB() : Strategy(2, "CPBB") { }
    void initializeGrid();
};

class StrategyGDFB : public Strategy
{
public:
    StrategyGDFB() : Strategy(3, "GDFB") { }
    void initializeProgram();
};

class StrategyGDBB : public Strategy
{
public:
    StrategyGDBB() : Strategy(4, "GDBB") { }
    void initializeProgram();
};

class StrategyGDBM : public Strategy
{
public:
    StrategyGDBM() : Strategy(5, "GDBM") { }
    void initializeProgram();
};
