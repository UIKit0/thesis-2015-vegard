#include <QGLWidget>
#include <QtWidgets>
#include <QtOpenGL>

#include "case.h"

#ifndef GLWIDGET_H
#define GLWIDGET_H
#endif

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

/**
 * Widget class.
 */
class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    GLuint program;
    Grid grid;
    Case1 c;
};
