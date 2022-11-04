#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_0_Core>
#include <QVector4D>
#include <vector>
#include <QFile>
#include <QApplication>
#include <QKeyEvent>
#include <QTimer>
#include <QElapsedTimer>
#include <array>
#include <QDebug>
#include <cmath>

class OpenGLWidget : public QOpenGLWidget, public QOpenGLFunctions_4_0_Core
{
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget *parent = nullptr);
    ~OpenGLWidget();

    GLuint vboVertices{0};
    GLuint vboColors{0};
    GLuint eboIndices{0};
    GLuint vao{0};

    std::vector<QVector4D> vertices;
    std::vector<QVector4D> colors;
    std::vector<GLuint> indices;

    GLuint shaderProgram{0};

    void createVBOs();
    void createShaders();

    void destroyVBOs();
    void destroyShaders();

    void changeDiagonal();

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    std::array<float, 2> playerPosOffset;
    std::array<float, 2> playerPos;

    float targetPosYOffset{2.0f};
    float targetPosY{0};

    QTimer timer;
    QElapsedTimer elapsedTime;

    bool shooting{false};
    std::array<float,2> projectilePos;
    std::array<float,2> projectilePosOffset;
    int numHits{0};

    bool enemyShooting{false};
    std::array<float, 2> enemyProjectilePos;
    std::array<float, 2> enemyProjectileOffset;
    bool playerAlive{true};

    bool targetHit{false};

signals:
    void updateHitsLabel(QString);
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
public slots:
    void animate();
};
#endif // OPENGLWIDGET_H
