#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QtWidgets>

#include "glwidget.h"

QT_BEGIN_NAMESPACE
class QSlider;
QT_END_NAMESPACE
class GLWidget;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

private:
    GLWidget *glWidget;
};

#endif
