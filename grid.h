#include "functor.h"

/**
 * Grid mesh.
 */
class Grid
{
public:
    Grid();
    Grid(int h, int w);
    ~Grid();
    Grid(const Grid &grid);
    Grid& operator=(const Grid &grid);

    void resize(int h, int w);

    GLuint getHeight() const;
    GLuint getWidth() const;
    GLuint getVerticesCount() const;
    GLuint getTexelsCount() const;
    GLuint getIndicesCount() const;

    GLfloat* getVertices() const;
    GLfloat* getTexels() const;
    GLuint* getIndices() const;

    void transform(const Functor &fn);
    void iTransform(const Functor &fn);

private:
    void initVertices();
    void initTexels();
    void initIndices();
    void init();
    void copy(const Grid &grid);
    void destruct();

    GLuint height;
    GLuint width;
    GLfloat* vertices;
    GLfloat* texels;
    GLuint* indices;
};
