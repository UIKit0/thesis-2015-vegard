#include "glwidget.h"

/**
 * Create a widget.
 */
GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
      grid(), c()
{
    program = 0;
}

GLWidget::~GLWidget()
{
}

/**
 * The recommended minimum size for the widget.
 */
QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

/**
 * This property holds the recommended size for the widget.
 */
QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

/**
 * Initialize the OpenGL environment.
 */
void GLWidget::initializeGL()
{
    c.initialize();
}

/**
 * Draw the OpenGL environment.
 */
void GLWidget::paintGL()
{
    c.paint();
}

/**
 * Resize the OpenGL environment.
 */
void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
}
