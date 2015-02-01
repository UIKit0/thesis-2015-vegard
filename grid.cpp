#include "grid.h"

#include <QDebug>

/**
 * Create a grid mesh.
 * @param h height.
 * @param h width.
 */
Grid::Grid(int h, int w)
    : height(h), width(w)
{
    initVertices();
    initTexels();
    initIndices();
}

Grid::~Grid() {
    delete[] vertices;
    delete[] texels;
    delete[] indices;
}

/**
 * Height.
 */
GLuint Grid::getHeight() const {
    return height;
}

/**
 * Width.
 */
GLuint Grid::getWidth() const {
    return width;
}

/**
 * Number of vertices.
 */
GLuint Grid::getVerticesCount() const {
    return height * width * 3;
}

/**
 * Number of texture coordinates.
 */
GLuint Grid::getTexelsCount() const {
    return height * width * 2;
}

/**
 * Number of vertex indices.
 */
GLuint Grid::getIndicesCount() const {
    return (height * width) + (width - 1) * (height - 2);
}

/**
 * Array of vertices.
 */
GLfloat* Grid::getVertices() const {
    return vertices;
}

/**
 * Array of texture coordinates.
 */
GLfloat* Grid::getTexels() const {
    return texels;
}

/**
 * Array of vertex indices.
 */
GLuint* Grid::getIndices() const {
    return indices;
}

/**
 * Transform vertices.
 * @param fn transformation functor.
 */
void Grid::transform(const Functor &fn) {
    int len = getVerticesCount();
    for(int i = 0; i < len; i += 3) {
        Coord coord(vertices[i], vertices[i + 1]);
        coord = fn(coord);
        vertices[i]     = coord.x;
        vertices[i + 1] = coord.y;
        vertices[i + 2] = 0.0;
    }
}

/**
 * Transform texture coordinates.
 * @param fn transformation functor.
 */
void Grid::iTransform(const Functor &fn) {
    int len = getTexelsCount();
    for(int i = 0; i < len; i += 2) {
        Coord coord(texels[i], texels[i + 1]);
        coord = fn(coord);
        texels[i]     = coord.x;
        texels[i + 1] = coord.y;
    }
}

/**
 * Initialize vertices.
 */
void Grid::initVertices() {
    vertices = new GLfloat[getVerticesCount()];
    GLuint i = 0;
    GLfloat h = height - 1;
    GLfloat w = width - 1;
    Shift shift(-w / 2.0, -h / 2.0);
    Scale scale(1 / w, 1 / h);

    for(GLuint row = 0; row < height; row++) {
        for(GLuint col = 0; col < width; col++) {
            Coord coord = scale(shift(Coord(col, row)));
            vertices[i++] = coord.x;
            vertices[i++] = coord.y;
            vertices[i++] = 0.0f;
        }
    }
}

/**
 * Initialize texture coordinates.
 */
void Grid::initTexels() {
    texels = new GLfloat[getTexelsCount()];
    GLuint i = 0;
    GLfloat h = height - 1;
    GLfloat w = width - 1;
    Scale scale(1 / w, 1 / h);

    for(GLuint row = 0; row < height; row++) {
        for(GLuint col = 0; col < width; col++) {
            Coord coord = scale(Coord(col, row));
            texels[i++] = coord.x;
            texels[i++] = coord.y;
        }
    }
}

/**
 * Initialize indices.
 */
void Grid::initIndices() {
    // http://stackoverflow.com/questions/5915753/generate-a-plane-with-triangle-strips
    GLuint iSize = getIndicesCount();
    indices = new GLuint[iSize];
    GLuint i = 0;

    // Indices for drawing cube faces using triangle strips. Triangle
    // strips can be connected by duplicating indices between the
    // strips. If connecting strips have opposite vertex order, then
    // the last index of the first strip and the first index of the
    // second strip need to be duplicated. If connecting strips have
    // same vertex order, then only the last index of the first strip
    // needs to be duplicated.
    for(GLuint row = 0; row < height - 1; row++) {
        if(row % 2 == 0) { // even rows
            for(GLuint col = 0; col < width; col++) {
                indices[i++] = col + row * width;
                indices[i++] = col + (row + 1) * width;
            }
        } else {           // odd rows
            for(GLuint col = width - 1; col > 0; col--) {
                indices[i++] = col + (row + 1) * width;
                indices[i++] = (col - 1) + row * width;
            }
        }
    }
}
