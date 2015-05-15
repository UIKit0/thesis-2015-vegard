#include "case.h"

/**
 * Create a case.
 */
Case::Case(const int n, const char *str)
    : id(n), title(str), grid(10, 10), program(0), initialized(false), times()
{
}

/**
 * Initialize the case.
 */
void Case::initialize()
{
    if(initialized) {
        return;
    }

    // qWarning() << "Initializing case " << id << ": " << title;

    initializeImage();
    initializeGrid();
    initializeProgram();
    initializeGL();

    initialized = true;
}

/**
 * Initialize the test image.
 */
void Case::initializeImage()
{
    image = QImage(":/test.png");
    image = image.mirrored();
    byteSwapImage(image, GL_UNSIGNED_BYTE);
}

/**
 * Initialize the grid mesh.
 */
void Case::initializeGrid()
{
}

/**
 * Initialize the OpenGL program.
 */
void Case::initializeProgram()
{
    createProgram(":/vshader1.glsl", ":/fshader1.glsl");
}

/**
 * Initialize the OpenGL environment.
 */
void Case::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    // glEnable(GL_MULTISAMPLE);
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
                 image.height(), image.width(),
                 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 image.constBits());

    // Set the filtering mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_LINEAR);

    // Get the sampler locations
    GLuint samplerLoc = glGetUniformLocation(program, "s_texture");

    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Set the sampler texture unit to 0
    glUniform1i(samplerLoc, 0);

    // Load texture coordinates
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, grid.getTexels());
    glEnableVertexAttribArray(0);

    // Load vertex positions
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, grid.getVertices());
    glEnableVertexAttribArray(1);
}

/**
 * Draw the OpenGL environment.
 */
void Case::paint()
{
    QElapsedTimer timer;
    timer.start();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLE_STRIP, grid.getIndicesCount(), GL_UNSIGNED_INT, grid.getIndices());
    addTime(timer.nsecsElapsed());
}

/**
 * Run the case a number of times.
 */
void Case::run()
{
    for(int i = 0; i < 1000; i++) {
        paint();
    }
}

/**
 * Add time measurement.
 */
void Case::addTime(int time)
{
    times << time;
    // qWarning() << "Added time " << time;
    // qWarning() << "Average time is " << (int)averageTime();
}

/**
 * Calculate the average time measurement.
 */
float Case::averageTime()
{
    if(times.size() <= 0) {
        return 0.0;
    }

    float sum = 0;
    for(int i = 0; i < times.size(); i++) {
        sum += times[i];
    }

    return sum / times.size();
}

/**
 * Print the average time measurement.
 */
void Case::printAverage()
{
    qWarning() << "Average time of case " << id << ": " << title << "\n"
               << averageTime() << "ns" << "\n";
}

/**
 * Create an OpenGL program object.
 */
void Case::createProgram(const char *vshader, const char *fshader)
{
    GLint linked = 0;

    // Create the program object
    program = glCreateProgram();

    if(program == 0) {
        return;
    }

    GLuint vertexShader = loadShaderFromResource(GL_VERTEX_SHADER, vshader);
    GLuint fragmentShader = loadShaderFromResource(GL_FRAGMENT_SHADER, fshader);

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
GLuint Case::loadShaderFromResource(GLenum type, QString resource)
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
GLuint Case::loadShader(GLenum type, const char *shaderSrc)
{
    GLuint shader;
    GLint compiled;

    // Create the shader object
    shader = glCreateShader(type);

    if(shader == 0) {
        qWarning() << "Error creating shader";
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

/**
 * Map from Qt's ARGB endianness-dependent format to
 * GL's big-endian RGBA layout.
 */
void Case::byteSwapImage(QImage &img, GLenum pixel_type)
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
 * Case 1: Forward mapping on the CPU
 */
void Case1::initializeGrid()
{
    Fish fish;
    QElapsedTimer timer;
    timer.start();
    grid.transform(fish);
    qWarning() << "Case " << id << " grid transformation: "
               << timer.nsecsElapsed() << "\n"; // 25000 ns
}

/**
 * Case 2: Backward mapping on the CPU
 */
void Case2::initializeGrid()
{
    FishInverse fishinverse;
    QElapsedTimer timer;
    timer.start();
    grid.iTransform(fishinverse);
    qWarning() << "Case " << id << " grid transformation: "
               << timer.nsecsElapsed() << "\n";
}

/**
 * Case 3: Forward mapping on the GPU
 */
void Case3::initializeProgram()
{
    createProgram(":/vshader2.glsl", ":/fshader1.glsl");
}

/**
 * Case 4: Backward mapping on the GPU
 */
void Case4::initializeProgram()
{
    createProgram(":/vshader1.glsl", ":/fshader2.glsl");
}

/**
 * Case 5: Supersampling
 */
void Case5::initializeProgram()
{
    createProgram(":/vshader1.glsl", ":/fshader3.glsl");
}
