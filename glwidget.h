/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

class QtLogo;

class PolarCoord;

class Coord
{
public:
    Coord(GLfloat xc, GLfloat yc);
    PolarCoord toPolarCoord();

    GLfloat x;
    GLfloat y;
};

class PolarCoord
{
public:
    PolarCoord(GLfloat rc, GLfloat thetac);
    Coord toCoord();

    GLfloat r;
    GLfloat theta;
};

class Grid
{
public:
    Grid(int h, int w);
    ~Grid();
    GLfloat* getVertices();
    GLfloat* getTexels();
    GLuint* getIndices();
    GLuint getVerticesCount();
    GLuint getTexelsCount();
    GLuint getIndicesCount();
    void transform(Coord fn(Coord));
    void iTransform(Coord fn(Coord));
    static Coord unity(Coord coord);
    static Coord half(Coord coord);
    static Coord fish(Coord coord);

    GLuint height;
    GLuint width;
    GLuint verticesCount;
    GLuint indicesCount;
    GLfloat* vertices;
    GLfloat* texels;
    GLuint* indices;

private:
    void initVertices();
    void initTexels();
    void initIndices();
};

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    GLuint loadShader(GLenum type, const char *shaderSrc);
    GLuint loadShaderFromResource(GLenum type, QString resource);
    void createProgram();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    GLuint program;
    Grid grid;

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
};

#endif
