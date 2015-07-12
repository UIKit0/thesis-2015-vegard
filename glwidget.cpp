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
    return QSize(700, 700);
}

/**
 * Initialize the OpenGL environment.
 */
void GLWidget::initializeGL()
{
    if(!measurements) {
        measurements = true;
        qWarning() << "";

        // s1.initialize();
        // s1.run();
        // s2.initialize();
        // s2.run();
        // s3.initialize();
        // s3.run();
        // s4.initialize();
        // s4.run();
        s5.initialize();
        s5.run();

        // s1.printMeasurements();
        // s2.printMeasurements();
        // s3.printMeasurements();
        // s4.printMeasurements();
        s5.printMeasurements();
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
