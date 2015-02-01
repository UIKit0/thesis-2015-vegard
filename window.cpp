#include "window.h"

/**
 * Create a window for the widget.
 */
Window::Window()
{
    glWidget = new GLWidget;

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(glWidget);
    setLayout(mainLayout);

    setWindowTitle(tr("Fisheye"));
}
