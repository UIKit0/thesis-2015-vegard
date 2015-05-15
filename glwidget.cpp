#include "glwidget.h"

/**
 * Create a widget.
 */
GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
      grid(), c1(), c2(), c3(), c4(), measurements(false)
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
    if(!measurements) {
        qWarning() << "";

        c1.initialize();
        c1.run();
        c2.initialize();
        c2.run();
        c3.initialize();
        c3.run();
        c4.initialize();
        c4.run();
        c5.initialize();
        c5.run();

        c1.printAverage();
        c2.printAverage();
        c3.printAverage();
        c4.printAverage();
        c5.printAverage();

        measurements = true;
    }
}

/**
 * Draw the OpenGL environment.
 */
void GLWidget::paintGL()
{
    c5.paint();
}
