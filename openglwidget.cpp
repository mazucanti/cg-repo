#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
}

OpenGLWidget::~OpenGLWidget()
{
    destroyVBOs();
    destroyShaders();
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0,0,0,0);
    qDebug("OpenGL Version: %s",glGetString(GL_VERSION));
    qDebug("GLSL Version: %s",glGetString(GL_SHADING_LANGUAGE_VERSION));

    connect(&timer, &QTimer::timeout, this, &OpenGLWidget::animate);
    timer.start(0);
    elapsedTime.start();

    createShaders();
    createVBOs();
}

void OpenGLWidget::resizeGL(int w, int h)
{
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    GLint localScaling{glGetUniformLocation(shaderProgram, "scaling")};
    GLint locTranslation{glGetUniformLocation(shaderProgram, "translation")};

    glBindVertexArray(vao);
    if(playerAlive)
    {
        //Player
        glUniform4f(locTranslation, playerPos[0], playerPos[1], 0, 0);
        glUniform1f(localScaling, 0.2f);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        //Target
        glUniform4f(locTranslation, 0.8f, targetPosY, 0, 0);
        glUniform1f(localScaling, 0.2f);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        //Projectile
        if(shooting){
            glUniform4f(locTranslation, projectilePos[0], projectilePos[1], 0, 0);
            glUniform1f(localScaling, 0.1f);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        }
        // Hit color
        if(targetHit)
        {
            glClearColor(1,0.4,0,1);
            targetHit = false;
        }
        else glClearColor(0,0,0,1);
        //Enemy projectile
        if(enemyShooting){
            glUniform4f(locTranslation, enemyProjectilePos[0], enemyProjectilePos[1], 0, 0);
            glUniform1f(localScaling, 0.1f);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        }
    }
    else
    {
        glClearColor(1,0,0,1);
    }

}


void OpenGLWidget::createShaders(){
    makeCurrent();
    destroyShaders();
    QFile vs(":/shaders/vshader1.glsl");
    QFile fs(":/shaders/fshader1.glsl");
    if(!vs.open(QFile::ReadOnly | QFile::Text)) return;//inserir mensagem de erro
    if(!fs.open(QFile::ReadOnly | QFile::Text)) return;//inserir mensagem de erro
    auto byteArrayVs{vs.readAll()};
    auto byteArrayFs{fs.readAll()};
    const char *c_strVs{byteArrayVs};
    const char *c_strFs{byteArrayFs};
    vs.close();
    fs.close();
    GLuint vertexShader{glCreateShader(GL_VERTEX_SHADER)};
    glShaderSource(vertexShader, 1, &c_strVs,0);
    glCompileShader(vertexShader);
    GLint isCompiled{0};
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
        GLint maxLength{0};
        glGetShaderiv(vertexShader,GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader,maxLength,&maxLength,&infoLog[0]);
        qDebug("%s",&infoLog[0]);
        glDeleteShader(vertexShader);
        return;
    }
    GLuint fragmentShader{glCreateShader(GL_FRAGMENT_SHADER)};
    glShaderSource(fragmentShader,1,&c_strFs,0);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&isCompiled);
    if(isCompiled == GL_FALSE)
    {
        GLint maxLength{0};
        glGetShaderiv(fragmentShader,GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader,maxLength,&maxLength,&infoLog[0]);
        qDebug("%s",&infoLog[0]);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return;
    }
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);
    GLint isLinked{0};
    glGetProgramiv(shaderProgram,GL_LINK_STATUS, &isLinked);
    if(isLinked == GL_FALSE)
    {
        GLint maxLength{0};
        glGetProgramiv(shaderProgram,GL_INFO_LOG_LENGTH,&maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(shaderProgram,maxLength,&maxLength, &infoLog[0]);
        qDebug("%s",&infoLog[0]);
        glDeleteProgram(shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return;
    }
    glDetachShader(shaderProgram,vertexShader);
    glDetachShader(shaderProgram,fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void OpenGLWidget::destroyShaders()
{
    makeCurrent();
    glDeleteProgram(shaderProgram);
}

void OpenGLWidget::createVBOs()
{
    makeCurrent();
    destroyVBOs();
    vertices.resize(4);
    colors.resize(4);
    indices.resize(6); //2*3
    vertices[0] = QVector4D(-0.5, -0.5 , 0, 1);
    vertices[1] = QVector4D( 0.5, -0.5 , 0, 1);
    vertices[2] = QVector4D( 0.5, 0.5 , 0, 1);
    vertices[3] = QVector4D(-0.5, 0.5 , 0, 1);
    // Create colors for the vertices
    colors[0] = QVector4D(1, 0, 1, 1); // Red
    colors[1] = QVector4D(0, 1, 1, 1); // Green
    colors[2] = QVector4D(1, 0, 1, 1); // Blue
    colors[3] = QVector4D(1, 1, 1, 1); // Yellow
    // Topology of the mesh ( square )
    indices[0] = 0; indices[1] = 1; indices[2] = 2;
    indices[3] = 2; indices[4] = 3; indices[5] = 0;

    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    glGenBuffers(1,&vboVertices);
    glBindBuffer(GL_ARRAY_BUFFER,vboVertices);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(QVector4D),vertices.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,nullptr);
    glEnableVertexAttribArray(0);
    glGenBuffers (1, &vboColors);
    glBindBuffer (GL_ARRAY_BUFFER,vboColors);
    glBufferData (GL_ARRAY_BUFFER,colors.size()*sizeof(QVector4D),colors.data(),
                  GL_STATIC_DRAW);
    glVertexAttribPointer (1, 4, GL_FLOAT , GL_FALSE , 0, nullptr);
    glEnableVertexAttribArray (1);
    glGenBuffers (1, &eboIndices);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER , eboIndices);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER , indices.size() * sizeof (GLuint), indices.data() ,
                  GL_STATIC_DRAW);
}

void OpenGLWidget::destroyVBOs()
{
    makeCurrent ();
    glDeleteBuffers(1, &vboVertices);
    glDeleteBuffers (1, &vboColors);
    glDeleteBuffers (1, &eboIndices);
    glDeleteVertexArrays (1, &vao);
    vboVertices=0;
    eboIndices=0;
    vboColors=0;
    vao=0;
}

void OpenGLWidget::changeDiagonal()
{
    makeCurrent();
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER , eboIndices);
    // glMappBuffer-> mac
    auto idx{static_cast<GLuint*>(glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER , 0,
                                                   indices.size()*sizeof(GL_UNSIGNED_INT),GL_MAP_WRITE_BIT))};
    idx[0] = 0; idx[1] = 1; idx[2] = 3;
    idx[3] = 1; idx[4] = 2; idx[5] = 3;
    glUnmapBuffer (GL_ELEMENT_ARRAY_BUFFER) ;
    update();
}

void OpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Up:
        playerPosOffset[1] = 2.0f;
        break;
    case Qt::Key_W:
        playerPosOffset[1] = 2.0f;
        break;
    case Qt::Key_D:
        playerPosOffset[0] = 2.0f;
        break;
    case Qt::Key_A:
        playerPosOffset[0] = -2.0f;
        break;
    case Qt::Key_Down:
        playerPosOffset[1] = -2.0f;
        break;
    case Qt::Key_S:
        playerPosOffset[1] = -2.0f;
        break;
    case Qt::Key_Left:
        playerPosOffset[0] = -2.0f;
        break;
    case Qt::Key_Right:
        playerPosOffset[0] = 2.0f;
        break;
    case Qt::Key_Escape:
        QApplication::quit();
        break;
    case Qt::Key_Space:
        if(!shooting)
        {
            shooting = true;
            projectilePos[0] = playerPos[0] + 0.1f;
            projectilePos[1] = playerPos[1];
            projectilePosOffset[0] = playerPosOffset[0];
            projectilePosOffset[1] = playerPosOffset[1];
        }
        break;
    }
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Up:
        playerPosOffset[1] = 0;
        break;
    case Qt::Key_W:
        playerPosOffset[1] = 0;
        break;
    case Qt::Key_D:
        playerPosOffset[0] = 0;
        break;
    case Qt::Key_A:
        playerPosOffset[0] = 0;
        break;
    case Qt::Key_Down:
        playerPosOffset[1] = 0;
        break;
    case Qt::Key_S:
        playerPosOffset[1] = 0;
        break;
    case Qt::Key_Left:
        playerPosOffset[0] = 0;
        break;
    case Qt::Key_Right:
        playerPosOffset[0] = 0;
        break;
    }
}

void OpenGLWidget::animate()
{
    float elTime{elapsedTime.restart()/1000.0f};
    if(playerAlive)
    {
        // PLAYER ANIMATION
        playerPos[0] += playerPosOffset[0] * elTime;
        playerPos[1] += playerPosOffset[1] * elTime;

        if(playerPos[1] > 0.8f) playerPos[1] = 0.8f;
        if(playerPos[1] < -0.8f) playerPos[1] = -0.8f;
        if(playerPos[0] > 0.8f) playerPos[0] = 0.8f;
        if(playerPos[0] < -0.8f) playerPos[0] = -0.8f;


        // TARGET ANIMATION
        targetPosY += targetPosYOffset * elTime;

        if(targetPosYOffset > 0)
        {
            if(targetPosY > 0.8f)
            {
                targetPosY = 0.8f;
                targetPosYOffset = -targetPosYOffset;
            }
        }
        else
        {
            if(targetPosY < -0.8f)
            {
                targetPosY = -0.8f;
                targetPosYOffset = -targetPosYOffset;
            }
        }
        if(!enemyShooting)
        {
            if(std::fabs(targetPosY - playerPos[1]) < 1.0f)
            {
                enemyProjectilePos[0] = 0.7f;
                enemyProjectilePos[1] = targetPosY;
                enemyProjectileOffset[0] = -1.5f;
                enemyProjectileOffset[1] = targetPosYOffset;
                enemyShooting = true;
            }
        }
        else
        {
            enemyProjectilePos[0] += (elTime * enemyProjectileOffset[0]);
            enemyProjectilePos[1] += (elTime * enemyProjectileOffset[1]);
            if(enemyProjectileOffset[1] > 0)
            {
                if(enemyProjectilePos[1] > 0.8f)
                {
                    enemyProjectilePos[1] = 0.8f;
                    enemyProjectileOffset[1] = -enemyProjectileOffset[1];
                }
            }
            if(enemyProjectileOffset[1] < 0)
            {
                if(enemyProjectilePos[1] < -0.8f)
                {
                    enemyProjectilePos[1] = -0.8f;
                    enemyProjectileOffset[1] = -enemyProjectileOffset[1];
                }
            }

            if(std::fabs(enemyProjectilePos[0] - playerPos[0]) < 0.125f
                    && std::fabs(enemyProjectilePos[1] - playerPos[1]) < 0.125f
                    )
            {
                playerAlive = false;
            }

            if(enemyProjectilePos[0] < -1.0f) enemyShooting = false;

        }


        // PROJECTILE ANIMATION
        if(shooting)
        {
            projectilePos[0] += 1.5f * elTime;
            projectilePos[1] += projectilePosOffset[1] * elTime;
            if(projectilePos[1] > 0.8f)
            {
                projectilePos[1] = 0.8f;
                projectilePosOffset[1] = -projectilePosOffset[1];
            }
            if(projectilePos[1] < -0.8f)
            {
                projectilePos[1] = -0.8f;
                projectilePosOffset[1] = -projectilePosOffset[1];
            }
            if(projectilePos[0] > 0.8f)
            {
                if(std::fabs(projectilePos[1] - targetPosY) < 0.3f)
                {
                    numHits++;
                    emit updateHitsLabel(QString("Acertos: %1").arg(numHits));
                    shooting = false;
                    targetHit = true;
                }
            }
            if(projectilePos[0] > 1.0f) shooting = false;
        }

        if(
                std::fabs(playerPos[0] - 0.8f) < 0.2
                && std::fabs(playerPos[1] - targetPosY) < 0.2
                ) playerAlive = false;
    }
    else
    {
        emit updateHitsLabel(QString("Você perdeu! (Aperte ESC)"));
    }
    update();
}
