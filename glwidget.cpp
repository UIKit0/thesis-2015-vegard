#include "glwidget.h"

/**
 * Create a widget.
 */
GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
      grid(), s1(), s2(), s3(), s4(), s5(), measurements(false)
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
        measurements = true;

        qWarning() << "";

        s1.initialize();
        s1.run();
        s2.initialize();
        s2.run();
        s3.initialize();
        s3.run();
        s4.initialize();
        s4.run();
        s5.initialize();
        s5.run();

        s1.printAverage();
        s2.printAverage();
        s3.printAverage();
        s4.printAverage();
        s5.printAverage();
    }
}

/**
 * Draw the OpenGL environment.
 */
void GLWidget::paintGL()
{
    // s1.paint();
    // s2.paint();
    // s3.paint();
    // s4.paint();
    s5.paint();
}
