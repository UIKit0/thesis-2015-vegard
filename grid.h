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
    void transform(Functor &fn);
    void iTransform(Functor &fn);

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
