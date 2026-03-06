#include "Qt5FFmpegRender.h"
#include <QDebug>
#include <QOpenGLShader>
#include <QResizeEvent>

Qt5FFmpegRender::Qt5FFmpegRender(QWidget *parent)
    : QOpenGLWidget(parent)
    , m_shaderProgram(nullptr)
    , m_vertexBuffer(QOpenGLBuffer::VertexBuffer)
    , m_textureDirty(false)
    , m_initialized(false)
    , m_textureId(0)
    , m_posAttr(-1)
    , m_texAttr(-1)
    , m_texUniform(-1)
{
    // 设置OpenGL ES格式
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGLES);
    format.setVersion(3, 0);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(0);
    format.setStencilBufferSize(0);
    format.setRedBufferSize(8);
    format.setGreenBufferSize(8);
    format.setBlueBufferSize(8);
    format.setAlphaBufferSize(0);
    setFormat(format);
}

Qt5FFmpegRender::~Qt5FFmpegRender()
{

    cleanup();
}

void Qt5FFmpegRender::initializeGL()
{
    if (m_initialized) {
        qWarning() << "Qt5ImageRender: Already initialized!";
        return;
    }

    // 初始化OpenGL函数
    initializeOpenGLFunctions();

    // 输出OpenGL信息
    qDebug() << "Qt5ImageRender: OpenGL Info -"
             << "Vendor:" << QString::fromUtf8((const char*)glGetString(GL_VENDOR))
             << "Renderer:" << QString::fromUtf8((const char*)glGetString(GL_RENDERER))
             << "Version:" << QString::fromUtf8((const char*)glGetString(GL_VERSION))
             << "GLSL Version:" << QString::fromUtf8((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glDisable(GL_DEPTH_TEST);

    // 初始化着色器
    if (!initializeShaderProgram()) {
        qCritical() << "Qt5ImageRender: Failed to initialize shader program!";
        return;
    }

    // 初始化缓冲区
    if (!initializeBuffers()) {
        qCritical() << "Qt5ImageRender: Failed to initialize buffers!";
        return;
    }

    m_initialized = true;
}

void Qt5FFmpegRender::paintGL()
{
    if (!m_initialized || !m_shaderProgram) {
        qWarning() << "Qt5ImageRender: Not properly initialized for painting";
        return;
    }

    glClear(GL_COLOR_BUFFER_BIT);

    m_shaderProgram->bind();

    // 更新纹理
    if (m_textureDirty && !m_image.isNull()) {
        createTexture(m_image);
        m_textureDirty = false;
    }

    // 渲染
    if (m_textureId != 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textureId);
        m_shaderProgram->setUniformValue(m_texUniform, 0);

        m_vertexBuffer.bind();

        // 设置顶点属性
        m_shaderProgram->enableAttributeArray(m_posAttr);
        m_shaderProgram->setAttributeBuffer(m_posAttr, GL_FLOAT, 0, 2, 4 * sizeof(GLfloat));

        m_shaderProgram->enableAttributeArray(m_texAttr);
        m_shaderProgram->setAttributeBuffer(m_texAttr, GL_FLOAT, 2 * sizeof(GLfloat), 2, 4 * sizeof(GLfloat));

        // 绘制
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        m_vertexBuffer.release();
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    m_shaderProgram->release();
}

void Qt5FFmpegRender::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Qt5FFmpegRender::displayImage(const QImage &image)
{
    if (image.isNull()) {
        qWarning() << "Qt5ImageRender: Received null image";
        return;
    }
    m_image = image.copy();
    m_textureDirty = true;
}

void Qt5FFmpegRender::createTexture(const QImage &image)
{
    // 删除现有纹理
    if (m_textureId != 0) {
        glDeleteTextures(1, &m_textureId);
        m_textureId = 0;
    }

    // 转换图像格式QResizeEvent
    QImage glImage = image.convertToFormat(QImage::Format_RGBA8888);

    // 如果需要垂直翻转
    glImage = glImage.mirrored(false, false);

    // 生成纹理
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    // 设置纹理参数 - 使用您指定的参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // 上传纹理数据
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glImage.width(), glImage.height(),
                 0, GL_RGBA, GL_UNSIGNED_BYTE, glImage.bits());

    glBindTexture(GL_TEXTURE_2D, 0);

}

void Qt5FFmpegRender::resizeEvent(QResizeEvent *event)
{
    if (isValid() && context()) {
        makeCurrent();
        resizeGL(event->size().width(), event->size().height());
        update();  // Trigger paintGL
        doneCurrent();
    }
}

bool Qt5FFmpegRender::initializeShaderProgram()
{
    if (m_shaderProgram) {
        delete m_shaderProgram;
    }

    m_shaderProgram = new QOpenGLShaderProgram();

    // 顶点着色器
    const char *vertexShaderSource = R"(
        attribute vec2 position;
        attribute vec2 texCoord;
        varying vec2 vTexCoord;
        void main() {
            gl_Position = vec4(position, 0.0, 1.0);
            vTexCoord = texCoord;
        }
    )";

    // 片段着色器 - 兼容性版本
    const char *fragmentShaderSource = R"(
        #ifdef GL_ES
        precision mediump float;
        #endif
        varying vec2 vTexCoord;
        uniform sampler2D texture;
        void main() {
            gl_FragColor = texture2D(texture, vTexCoord);
        }
    )";

    // 编译着色器
    if (!m_shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource)) {
        qCritical() << "Qt5ImageRender: Vertex shader compilation failed:" << m_shaderProgram->log();
        return false;
    }

    if (!m_shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource)) {
        qCritical() << "Qt5ImageRender: Fragment shader compilation failed:" << m_shaderProgram->log();
        return false;
    }

    // 链接程序
    if (!m_shaderProgram->link()) {
        qCritical() << "Qt5ImageRender: Shader program linking failed:" << m_shaderProgram->log();
        return false;
    }

    // 获取属性位置
    m_posAttr = m_shaderProgram->attributeLocation("position");
    m_texAttr = m_shaderProgram->attributeLocation("texCoord");
    m_texUniform = m_shaderProgram->uniformLocation("texture");

    return true;
}

bool Qt5FFmpegRender::initializeBuffers()
{
    // 顶点数据：位置(2) + 纹理坐标(2)
    // 使用三角形条带 (triangle strip) 来绘制矩形
    GLfloat vertices[] = {
        // 位置      纹理坐标
        -1.0f, -1.0f, 0.0f, 1.0f,  // 左下
        1.0f, -1.0f, 1.0f, 1.0f,  // 右下
        -1.0f,  1.0f, 0.0f, 0.0f,  // 左上
        1.0f,  1.0f, 1.0f, 0.0f   // 右上
    };

    // 创建并配置顶点缓冲区
    if (!m_vertexBuffer.create()) {
        qCritical() << "Qt5ImageRender: Failed to create vertex buffer";
        return false;
    }

    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(vertices, sizeof(vertices));
    m_vertexBuffer.release();
    return true;
}

void Qt5FFmpegRender::cleanup()
{
    makeCurrent();

    // 删除纹理
    if (m_textureId != 0) {
        glDeleteTextures(1, &m_textureId);
        m_textureId = 0;
    }

    delete m_shaderProgram;
    m_shaderProgram = nullptr;

    if (m_vertexBuffer.isCreated()) {
        m_vertexBuffer.destroy();
    }

    doneCurrent();

    m_initialized = false;
}
QOpenGLTexture * Qt5FFmpegRender::loadTexture(const QImage& image)
{
    QOpenGLTexture* texture =nullptr;
    if(image.isNull())return nullptr;
    // 转换为RGBA格式
    QImage glImage = image.convertToFormat(QImage::Format_RGBA8888);

    // 检查OpenGL上下文
    if (!QOpenGLContext::currentContext()) {
        qDebug() << "No OpenGL context for texture creation";
        return nullptr;
    }

    // 创建纹理 - 使用镜像来修正OpenGL纹理坐标
    texture = new QOpenGLTexture(glImage.mirrored(true, false));
    if (!texture || !texture->textureId()) {
        qDebug() << "Failed to create OpenGL texture";
        delete texture;
        return nullptr;
    }

    texture->setMinificationFilter(QOpenGLTexture::Linear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::ClampToEdge);
    return texture;
}
