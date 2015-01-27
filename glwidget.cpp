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
#include <cmath>
#include <QDebug>
#include <QTextStream>

#include <QtWidgets>
#include <QtOpenGL>

#include <math.h>

#include "glwidget.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

Coord::Coord(GLfloat xc, GLfloat yc)
    : x(xc), y(yc)
{
}

PolarCoord Coord::toPolarCoord() {
    GLfloat r = sqrt(x * x + y * y);
    GLfloat theta = atan2(y, x);
    return PolarCoord(r, theta);
}

Coord PolarCoord::toCoord() {
    GLfloat x = r * cos(theta);
    GLfloat y = r * sin(theta);
    return Coord(x, y);
}

PolarCoord::PolarCoord(GLfloat rc, GLfloat thetac)
    : r(rc), theta(thetac)
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

// Indices for drawing cube faces using triangle strips.
// Triangle strips can be connected by duplicating indices
// between the strips. If connecting strips have opposite
// vertex order then last index of the first strip and first
// index of the second strip needs to be duplicated. If
// connecting strips have same vertex order then only last
// index of the first strip needs to be duplicated.
void Grid::initVertices() {
    vertices = new GLfloat[getVerticesCount()];
    GLuint i = 0;

    for(GLuint row = 0; row < height; row++) {
        for(GLuint col = 0; col < width; col++) {
            GLfloat h = height - 1;
            GLfloat w = width - 1;
            GLfloat r = (row - h / 2.0) / h;
            GLfloat c = (col - w / 2.0) / w;
            vertices[i++] = c;
            vertices[i++] = r;
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
}

GLfloat* Grid::getVertices() {
    return vertices;
}

GLuint* Grid::getIndices() {
    return indices;
}

void Grid::transform(Coord fn(Coord)) {
    int len = getVerticesCount();
    for(int i = 0; i < len; i += 3) {
        Coord coord(vertices[i], vertices[i + 1]);
        coord = fn(coord);
        vertices[i]     = coord.x;
        vertices[i + 1] = coord.y;
        vertices[i + 2] = 0.0;
    }
}

Coord Grid::unity(Coord coord) {
    return coord;
}

Coord Grid::half(Coord coord) {
    coord.x = coord.x / 2;
    coord.y = coord.y / 2;
    return coord;
}

Coord Grid::fish(Coord coord) {
    GLuint h = 0;
    GLuint w = 0;
    Coord c = Coord(coord.x - w / 2, coord.y - h / 2);
    PolarCoord p = c.toPolarCoord();
    // GLfloat rr = p.r + 0.1 * p.r * p.r;
    GLfloat rr = p.r - 0.5 * p.r * p.r;
    PolarCoord pp = PolarCoord(rr, p.theta);
    Coord cc = pp.toCoord();
    return Coord(cc.x + w / 2, cc.y + h / 2);
}

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
      grid(10, 10)
{
    program = 0;
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

GLuint GLWidget::loadShader(GLenum type, const char *shaderSrc)
{
    GLuint shader;
    GLint compiled;

    // Create the shader object
    shader = glCreateShader(type);

    if(shader == 0) {
        return 0;
    }

    // Load the shader source
    glShaderSource(shader, 1, &shaderSrc, NULL);

    // Compile the shader
    glCompileShader(shader);

    // Check the compile status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if(!compiled) {
        GLint infoLen = 0;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

        if(infoLen > 1) {
            qWarning() << "Error compiling shader";

            // char* infoLog = malloc(sizeof(char) * infoLen);

            // glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            // esLogMessage("Error compiling shader:\n%s\n", infoLog);

            // free(infoLog);
        }

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint GLWidget::loadShaderFromResource(GLenum type, QString resource)
{
    QFile shader(resource);
    shader.open(QIODevice::ReadOnly);
    QByteArray glsl = shader.readAll();
    shader.close();
    return loadShader(type, glsl.data());
}

void GLWidget::createProgram()
{
    GLint linked = 0;

    // Create the program object
    program = glCreateProgram();

    if(program == 0) {
        return;
    }

    GLuint vertexShader = loadShaderFromResource(GL_VERTEX_SHADER, ":/vshader.glsl");
    GLuint fragmentShader = loadShaderFromResource(GL_FRAGMENT_SHADER, ":/fshader.glsl");

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    // Link the program
    glLinkProgram(program);

    // Check the link status
    glGetProgramiv(program, GL_LINK_STATUS, &linked);

    if(!linked) {
        GLint infoLen = 0;

        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);

        if(infoLen > 1) {
            qWarning() << "Error linking program";

            // char* infoLog = malloc(sizeof(char) * infoLen);
            // glGetProgramInfoLog(program, infoLen, NULL, infoLog);

            // esLogMessage("Error linking program:\n%s\n", infoLog);
            // free(infoLog);
        }

        glDeleteProgram(program);
    }
}

void GLWidget::initializeGL()
{
    createProgram();
    // GLuint shader = loadShaderFromResource(GL_VERTEX_SHADER, ":/vshader.glsl");
    // QFile shader(":/vshader.glsl");
    // shader.open(QIODevice::ReadOnly);
    // QByteArray glsl = shader.readAll();
    // shader.close();
    // qWarning() << shader.exists();
    // qWarning() << glsl.isEmpty();
    // qWarning() << glsl.data();
    // qWarning() << shader;
    QColor qtBlack = Qt::black;
    qglClearColor(qtBlack);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    grid.transform(Grid::fish);
    glUseProgram(program);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glLoadIdentity();
    // glTranslatef(0.0, 0.0, -10.0);
    // glRotatef(0, 1.0, 0.0, 0.0);
    // glRotatef(0, 0.0, 1.0, 0.0);
    // glRotatef(0, 0.0, 0.0, 1.0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, grid.vertices);
    glEnableVertexAttribArray(0);
    glDrawElements(GL_TRIANGLE_STRIP, grid.indicesCount, GL_UNSIGNED_INT, grid.indices);

    // qWarning() << grid.indicesCount;

    // glEnableClientState(GL_VERTEX_ARRAY);
    // glVertexPointer(3, GL_FLOAT, 0, grid.vertices);
    // glDrawElements(GL_TRIANGLE_STRIP, grid.indicesCount, GL_UNSIGNED_INT, grid.indices);
    // glDisableClientState(GL_VERTEX_ARRAY);
}

void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

//     glMatrixMode(GL_PROJECTION);
//     glLoadIdentity();
// #ifdef QT_OPENGL_ES_1
//     glOrthof(-1, +10, -1, +10, 4.0, 15.0);
// #else
//     glOrtho(-1, +10, -1, +10, 4.0, 15.0);
// #endif
//     glMatrixMode(GL_MODELVIEW);
}
