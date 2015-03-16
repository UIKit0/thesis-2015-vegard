#include <QtOpenGL>

#include "grid.h"

#ifndef GLWIDGET_H
#define GLWIDGET_H
#endif

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

/**
 * Case class.
 */
class Case
{
public:
    Case();
    ~Case();

    void initialize();
    void paint();

protected:
    void createProgram(const char *vshader, const char *fshader);
    GLuint loadShaderFromResource(GLenum type, QString resource);
    GLuint loadShader(GLenum type, const char *shaderSrc);

    GLuint program;
};

/**
 * Case class.
 */
class Case1 : public Case
{
public:
    Case1();
    ~Case1();

    void initialize();
    void paint();

protected:
    Grid grid;
};
