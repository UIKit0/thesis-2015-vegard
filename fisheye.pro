HEADERS       = glwidget.h \
                window.h \
                point.h \
                functor.h \
                grid.h
SOURCES       = glwidget.cpp \
                main.cpp \
                window.cpp \
                point.cpp \
                functor.cpp \
                grid.cpp
RESOURCES    += shaders.qrc \
                textures.qrc
OTHER_FILES  += fshader.glsl \
                vshader.glsl
QT           += opengl widgets

contains(QT_CONFIG, opengles.) {
    contains(QT_CONFIG, angle): \
        warning("Qt was built with ANGLE, which provides only OpenGL ES 2.0 on top of DirectX 9.0c")
    error("This example requires Qt to be configured with -opengl desktop")
}
