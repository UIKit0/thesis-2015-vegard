#include "functor.h"

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
    void transform(const Functor &fn);
    void iTransform(const Functor &fn);

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
