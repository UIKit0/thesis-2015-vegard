HEADERS       = glwidget.h \
                window.h \
                point.h \
                functor.h \
                grid.h \
                case.h
SOURCES       = glwidget.cpp \
                main.cpp \
                window.cpp \
                point.cpp \
                functor.cpp \
                grid.cpp \
                case.cpp
RESOURCES    += shaders.qrc \
                textures.qrc
OTHER_FILES  += fshader1.glsl \
                fshader2.glsl \
                fshader3.glsl \
                vshader1.glsl \
                vshader2.glsl
QT           += opengl widgets

contains(QT_CONFIG, opengles.) {
    contains(QT_CONFIG, angle): \
        warning("Qt was built with ANGLE, which provides only OpenGL ES 2.0 on top of DirectX 9.0c")
    error("This example requires Qt to be configured with -opengl desktop")
}
