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

#include <iostream>
#include <QDebug>
#include <QTextStream>

#include <QtWidgets>
#include <QtOpenGL>

#include <math.h>

#include "glwidget.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

Coordinate::Coordinate(GLfloat xc, GLfloat yc, GLfloat zc)
    : x(xc), y(yc), z(zc)
{
}

// http://stackoverflow.com/questions/5915753/generate-a-plane-with-triangle-strips
Grid::Grid(int h, int w) {
    height = h;
    width = w;

    initVertices();
    initIndices();
    verticesCount = getVerticesCount();
    indicesCount = getIndicesCount();
}

Grid::~Grid() {
    delete[] vertices;
    delete[] indices;
}

GLuint Grid::getVerticesCount() {
    return height * width * 3;
}

GLuint Grid::getIndicesCount() {
    return (height * width) + (width - 1) * (height - 2);
}

void Grid::initVertices() {
    vertices = new GLfloat[getVerticesCount()];
    GLuint i = 0;

    for(GLuint row = 0; row < height; row++) {
        for(GLuint col = 0; col < width; col++) {
            vertices[i++] = (GLfloat)col;
            vertices[i++] = (GLfloat)row;
            vertices[i++] = 0.0f;
        }
    }
}

void Grid::initIndices() {
    GLuint iSize = getIndicesCount();
    indices = new GLuint[iSize];
    GLuint i = 0;

    for(GLuint row = 0; row < height - 1; row++) {
        if((row & 1) == 0) { // even rows
            for(GLuint col = 0; col < width; col++) {
                indices[i++] = col + row * width;
                indices[i++] = col + (row + 1) * width;
            }
        } else { // odd rows
            for(GLuint col = width - 1; col > 0; col--) {
                indices[i++] = col + (row + 1) * width;
                indices[i++] = col - 1 + + row * width;
            }
        }
    }

    // if((mHeight & 1) && mHeight > 2) {
    //     mpIndices[i++] = (mHeight-1) * mWidth;
    // }
}

GLfloat* Grid::getVertices() {
    return vertices;
}

GLuint* Grid::getIndices() {
    return indices;
}

void Grid::transform(Coordinate fn(Coordinate)) {
    int len = getVerticesCount();
    for(int i = 0; i < len; i += 3) {
        Coordinate coord(vertices[i], vertices[i + 1], vertices[i + 2]);
        coord = fn(coord);
        vertices[i]     = coord.x;
        vertices[i + 1] = coord.y;
        vertices[i + 2] = coord.z;
    }
}

Coordinate Grid::unity(Coordinate coord) {
    return coord;
}

Coordinate Grid::half(Coordinate coord) {
    coord.x = coord.x / 2;
    coord.y = coord.y / 2;
    coord.z = coord.z / 2;
    return coord;
}

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
      grid(10, 10)
{
}

GLWidget::~GLWidget()
{
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

void GLWidget::initializeGL()
{
    QColor qtBlack = Qt::black;
    qglClearColor(qtBlack);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    grid.transform(Grid::half);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(0, 1.0, 0.0, 0.0);
    glRotatef(0, 0.0, 1.0, 0.0);
    glRotatef(0, 0.0, 0.0, 1.0);

    // qWarning() << grid.indicesCount;

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, grid.vertices);
    glDrawElements(GL_TRIANGLE_STRIP, grid.indicesCount, GL_UNSIGNED_INT, grid.indices);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-1, +10, -1, +10, 4.0, 15.0);
#else
    glOrtho(-1, +10, -1, +10, 4.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}
