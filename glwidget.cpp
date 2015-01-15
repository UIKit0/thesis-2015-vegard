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

#include <QtWidgets>
#include <QtOpenGL>

#include <math.h>

#include "glwidget.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

// http://stackoverflow.com/questions/5915753/generate-a-plane-with-triangle-strips

// GLuint width;
// GLuint height;
GLfloat* vertices = 0;
GLuint* indices = 0;

GLuint getVerticesCount(GLuint width, GLuint height) {
    return width * height * 3;
}

GLuint getIndicesCount(GLuint width, GLuint height) {
    return (width * height) + (width - 1) * (height - 2);
}

void initVertices(GLuint width, GLuint height) {
    vertices = new GLfloat[getVerticesCount(width, height)];
    GLuint i = 0;

    for(GLuint row = 0; row < height; row++) {
        for(GLuint col = 0; col < width; col++) {
            vertices[i++] = (GLfloat)col;
            vertices[i++] = (GLfloat)row;
            vertices[i++] = 0.0f;
        }
    }
}

void initIndices(GLuint width, GLuint height) {
    GLuint iSize = getIndicesCount(width, height);
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

GLfloat* getVertices(GLuint width, GLuint height) {
    if(!vertices) {
        initVertices(width, height);
    }

    return vertices;
}

GLuint* getIndices(GLuint width, GLuint height) {
    if(!indices) {
        initIndices(width, height);
    }

    return indices;
}

// void render() {
//     glEnableClientState(GL_VERTEX_ARRAY);
//     glVertexPointer(3, GL_FLOAT, 0, getVertices(width, height));
//     glDrawElements(GL_TRIANGLE_STRIP, getIndicesCount(width, height), GL_UNSIGNED_INT, getIndices(width, height));
//     glDisableClientState(GL_VERTEX_ARRAY);
// }

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
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
    // glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(0, 1.0, 0.0, 0.0);
    glRotatef(0, 0.0, 1.0, 0.0);
    glRotatef(0, 0.0, 0.0, 1.0);

    // GLfloat vVertices[] = {  0.0f,  0.5f,  0.0f,
    //                         -0.5f, -0.5f,  0.0f,
    //                          0.5f, -0.5f,  0.0f };

    // GLfloat vVertices[] = {  0.0f,  0.7f,  0.1f,
    //                         -0.4f, -0.6f,  0.2f,
    //                          0.3f, -0.8f,  0.3f };

    // glPolygonMode( GL_FRONT, GL_LINE );
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    // glDrawArrays(GL_TRIANGLES, 0, 3);

    // GLfloat vertices[] = { 0.0f, 0.0f, 0.0f,   // 0
    //                        0.0f, 1.0f, 0.0f,   // 1
    //                        0.0f, 2.0f, 0.0f,   // 2
    //                        0.0f, 3.0f, 0.0f,   // 3

    //                        1.0f, 0.0f, 0.0f,   // 4
    //                        1.0f, 1.0f, 0.0f,   // 5
    //                        1.0f, 2.0f, 0.0f,   // 6
    //                        1.0f, 3.0f, 0.0f,   // 7

    //                        2.0f, 0.0f, 0.0f,   // 8
    //                        2.0f, 1.0f, 0.0f,   // 9
    //                        2.0f, 2.0f, 0.0f,   // 10
    //                        2.0f, 3.0f, 0.0f,   // 11

    //                        3.0f, 0.0f, 0.0f,   // 12
    //                        3.0f, 1.0f, 0.0f,   // 13
    //                        3.0f, 2.0f, 0.0f,   // 14
    //                        3.0f, 3.0f, 0.0f }; // 15

    // GLfloat vertices[] = { 0.0f, 0.0f, 0.0f,   // 0
    //                        1.0f, 0.0f, 0.0f,   // 1
    //                        2.0f, 0.0f, 0.0f,   // 2
    //                        3.0f, 0.0f, 0.0f,   // 3

    //                        0.0f, 1.0f, 0.0f,   // 4
    //                        1.0f, 1.0f, 0.0f,   // 5
    //                        2.0f, 1.0f, 0.0f,   // 6
    //                        3.0f, 1.0f, 0.0f,   // 7

    //                        0.0f, 2.0f, 0.0f,   // 8
    //                        1.0f, 2.0f, 0.0f,   // 9
    //                        2.0f, 2.0f, 0.0f,   // 10
    //                        3.0f, 2.0f, 0.0f,   // 11

    //                        0.0f, 3.0f, 0.0f,   // 12
    //                        1.0f, 3.0f, 0.0f,   // 13
    //                        2.0f, 3.0f, 0.0f,   // 14
    //                        3.0f, 3.0f, 0.0f }; // 15

    // GLuint indices[] = { 0, 4, 1, 5, 2, 6, 3, 7 };
    // GLuint indices[] = { 0, 4, 1 };
    // GLuint indices[] = { 0, 1, 4 };

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glEnableClientState(GL_VERTEX_ARRAY);
    // glVertexPointer(3, GL_FLOAT, 0, vertices);
    // glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_INT, indices);
    // glDisableClientState(GL_VERTEX_ARRAY);

    GLuint width = 10;
    GLuint height = 10;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, getVertices(width, height));
    glDrawElements(GL_TRIANGLE_STRIP, getIndicesCount(width, height), GL_UNSIGNED_INT, getIndices(width, height));
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
