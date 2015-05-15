#include <QGLWidget>
#include <QtWidgets>
#include <QtOpenGL>

#include "case.h"

#ifndef GLWIDGET_H
#define GLWIDGET_H
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

    GLuint program;
    Grid grid;
    Case1 c1;
    Case2 c2;
    Case3 c3;
    Case4 c4;
    Case5 c5;
    bool measurements;
};
