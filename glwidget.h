#include <QGLWidget>
#include <QtWidgets>
#include <QtOpenGL>

#include "grid.h"

#ifndef GLWIDGET_H
#define GLWIDGET_H
#endif

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    GLuint loadShader(GLenum type, const char *shaderSrc);
    GLuint loadShaderFromResource(GLenum type, QString resource);
    void createProgram();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    GLuint program;
    Grid grid;

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
};
