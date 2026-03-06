#ifndef QT5FFMPEGRENDER_H
#define QT5FFMPEGRENDER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QImage>
#include <QOpenGLTexture>

class Qt5FFmpegRender : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit Qt5FFmpegRender(QWidget *parent = nullptr);
    ~Qt5FFmpegRender();
    static QOpenGLTexture *loadTexture(const QImage &image);

public slots:
    void displayImage(const QImage &image);
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void resizeEvent(QResizeEvent *event);
private:
    bool initializeShaderProgram();
    bool initializeBuffers();
    void cleanup();
    void createTexture(const QImage &image);
    QOpenGLShaderProgram *m_shaderProgram;
    QOpenGLBuffer m_vertexBuffer;
    QImage m_image;
    bool m_textureDirty;
    bool m_initialized;

    GLuint m_textureId;
    int m_posAttr;
    int m_texAttr;
    int m_texUniform;

};

#endif // QT5FFMPEGRENDER_H
