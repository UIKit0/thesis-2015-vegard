#include "window.h"

/**
 * Create a window for the widget.
 */
Window::Window()
{
    glWidget = new GLWidget;
    scrollArea = new QScrollArea;

    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(glWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(glWidget);
    // mainLayout->addWidget(scrollArea);
    setLayout(mainLayout);

    setWindowTitle(tr("Fisheye"));
}
