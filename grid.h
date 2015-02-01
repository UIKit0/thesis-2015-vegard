#include "functor.h"

/**
 * Grid mesh.
 */
class Grid
{
public:
    Grid(int h, int w);
    ~Grid();

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

    GLuint height;
    GLuint width;
    GLfloat* vertices;
    GLfloat* texels;
    GLuint* indices;
};
