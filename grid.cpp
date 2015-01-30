#include "grid.h"

// http://stackoverflow.com/questions/5915753/generate-a-plane-with-triangle-strips
Grid::Grid(int h, int w)
    : height(h), width(w)
{
    initVertices();
    initTexels();
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

GLuint Grid::getTexelsCount() {
    return height * width * 2;
}

GLuint Grid::getIndicesCount() {
    return (height * width) + (width - 1) * (height - 2);
}

void Grid::initVertices() {
    vertices = new GLfloat[getVerticesCount()];
    GLuint i = 0;
    GLfloat h = height - 1;
    GLfloat w = width - 1;

    for(GLuint row = 0; row < height; row++) {
        for(GLuint col = 0; col < width; col++) {
            GLfloat r = (row - h / 2.0) / h;
            GLfloat c = (col - w / 2.0) / w;
            vertices[i++] = c;
            vertices[i++] = r;
            vertices[i++] = 0.0f;
        }
    }
}

void Grid::initTexels() {
    texels = new GLfloat[getTexelsCount()];
    GLuint i = 0;
    GLfloat h = height - 1;
    GLfloat w = width - 1;

    for(GLuint row = 0; row < height; row++) {
        for(GLuint col = 0; col < width; col++) {
            GLfloat r = row / h;
            GLfloat c = col / w;
            texels[i++] = c;
            texels[i++] = r;
        }
    }
}

// Indices for drawing cube faces using triangle strips. Triangle
// strips can be connected by duplicating indices between the strips.
// If connecting strips have opposite vertex order, then the last
// index of the first strip and the first index of the second strip
// need to be duplicated. If connecting strips have same vertex order,
// then only the last index of the first strip needs to be duplicated.
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
                indices[i++] = (col - 1) + row * width;
            }
        }
    }
}

GLfloat* Grid::getVertices() {
    return vertices;
}

GLfloat* Grid::getTexels() {
    return texels;
}

GLuint* Grid::getIndices() {
    return indices;
}

void Grid::transform(Functor &fn) {
    int len = getVerticesCount();
    for(int i = 0; i < len; i += 3) {
        Coord coord(vertices[i], vertices[i + 1]);
        coord = fn(coord);
        vertices[i]     = coord.x;
        vertices[i + 1] = coord.y;
        vertices[i + 2] = 0.0;
    }
}

void Grid::iTransform(Functor &fn) {
    int len = getTexelsCount();
    for(int i = 0; i < len; i += 2) {
        Coord coord(texels[i], texels[i + 1]);
        coord = fn(coord);
        texels[i]     = coord.x;
        texels[i + 1] = coord.y;
    }
}
