#include "glwidget.h"

/**
 * Map from Qt's ARGB endianness-dependent format to
 * GL's big-endian RGBA layout.
 */
static inline void qgl_byteSwapImage(QImage &img, GLenum pixel_type)
{
    const int width = img.width();
    const int height = img.height();

    if(pixel_type == GL_UNSIGNED_INT_8_8_8_8_REV ||
       (pixel_type == GL_UNSIGNED_BYTE &&
        QSysInfo::ByteOrder == QSysInfo::LittleEndian)) {
        for(int i = 0; i < height; ++i) {
            uint *p = (uint*)img.scanLine(i);
            for(int x = 0; x < width; ++x) {
                p[x] = ((p[x] << 16) & 0xff0000) |
                       ((p[x] >> 16) & 0xff) |
                       (p[x] & 0xff00ff00);
            }
        }
    } else {
        for(int i = 0; i < height; ++i) {
            uint *p = (uint*)img.scanLine(i);
            for(int x = 0; x < width; ++x) {
                p[x] = (p[x] << 8) | ((p[x] >> 24) & 0xff);
            }
        }
    }
}

/**
 * Create a widget.
 */
GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
      grid()
{
    program = 0;
}

GLWidget::~GLWidget()
{
}

/**
 * The recommended minimum size for the widget.
 */
QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

/**
 * This property holds the recommended size for the widget.
 */
QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

/**
 * Initialize the OpenGL environment.
 */
void GLWidget::initializeGL()
{
    QImage testImage(":/test.png");
    testImage = testImage.mirrored();
    qgl_byteSwapImage(testImage, GL_UNSIGNED_BYTE);

    grid = Grid(10, 10);
    // grid = Grid(testImage.height(), testImage.width());

    Fisheye fisheye;
    grid.transform(fisheye);
    // grid.transform(fisheye.inverse);

    createProgram();
    QColor qtBlack = Qt::black;
    qglClearColor(qtBlack);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    glUseProgram(program);

    // Texture object handle
    GLuint textureId;

    // Use tightly packed data
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Generate a texture object
    glGenTextures(1, &textureId);

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Load the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 testImage.height(), testImage.width(),
                 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 testImage.constBits());

    // Set the filtering mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
    //                 GL_MIRRORED_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R,
    //                 GL_MIRRORED_REPEAT);

    // Get the sampler locations
    GLuint samplerLoc = glGetUniformLocation(program, "s_texture");

    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Set the sampler texture unit to 0
    glUniform1i(samplerLoc, 0);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, grid.getTexels());
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, grid.getVertices());
    glEnableVertexAttribArray(1);
}

/**
 * Draw the OpenGL environment.
 */
void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLE_STRIP, grid.getIndicesCount(), GL_UNSIGNED_INT, grid.getIndices());
}

/**
 * Resize the OpenGL environment.
 */
void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
}

/**
 * Create an OpenGL program object.
 */
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
            char* infoLog = new char[infoLen];
            glGetProgramInfoLog(program, infoLen, NULL, infoLog);
            qWarning() << "Error linking program: " << infoLog;
            delete[] infoLog;
        }

        glDeleteProgram(program);
    }
}

/**
 * Load a shader from a resource file.
 * @param type the type, GL_VERTEX_SHADER or GL_FRAGMENT_SHADER.
 * @param resource path to resource.
 */
GLuint GLWidget::loadShaderFromResource(GLenum type, QString resource)
{
    QFile shader(resource);
    shader.open(QIODevice::ReadOnly);
    QByteArray glsl = shader.readAll();
    shader.close();
    return loadShader(type, glsl.data());
}

/**
 * Load a shader from a source string.
 * @param type the type, GL_VERTEX_SHADER or GL_FRAGMENT_SHADER.
 * @param shaderSrc string containing the shader's source.
 */
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
            char* infoLog = new char[infoLen];
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            qWarning() << "Error compiling shader: " << infoLog;
            delete[] infoLog;
        }

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}
