#include <QGLWidget>
#include <QtWidgets>
#include <QtOpenGL>

#include "strategy.h"

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
    StrategyCPFB s1;
    StrategyCPBB s2;
    StrategyGDFB s3;
    StrategyGDBB s4;
    StrategyGDBM s5;
    bool measurements;
};
