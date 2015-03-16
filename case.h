#include <QtOpenGL>

#include "grid.h"

#ifndef GLWIDGET_H
#define GLWIDGET_H
#endif

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

/**
 * Widget class.
 */
class Case
{
public:
    Case();
    ~Case();

    void initialize();
    void paint();

protected:
    void createProgram();
    GLuint loadShaderFromResource(GLenum type, QString resource);
    GLuint loadShader(GLenum type, const char *shaderSrc);

    GLuint program;
    Grid grid;
};
