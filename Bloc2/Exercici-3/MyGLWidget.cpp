// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::ini_camera (glm::vec3 OBS, glm::vec3 VRP, glm::vec3 up, float FOV, 
                            float ra, float znear, float zfar) {
    projectTransform(FOV, ra, znear, zfar);
    viewTransform(OBS, VRP, up);
}


void MyGLWidget::initializeGL () {
    BL2GLWidget::initializeGL();
    //projectTransform() y viewTransform() solo 
    //se ha de llamar una vez, en initializeGL ya va bien.
    ini_camera(glm::vec3(0,0,1), glm::vec3(0,0,0), glm::vec3(1,0,0), M_PI/2.0f,
                1.0, 0.4, 3);       
    //CUIDADO: se ha de llamar DESPUÉS de carregaShaders, sino da core dumped.
} 


void MyGLWidget::carregaShaders () {
    BL2GLWidget::carregaShaders();
    //Apuntador a matriz de proyección
    projLoc = glGetUniformLocation(program->programId(), "proj");
    viewLoc = glGetUniformLocation(program->programId(), "view");
}

void MyGLWidget::projectTransform (float fov, float ra, float znear, float zfar) {
    // glm::perspective (FOV en radianes, ra window, znear, zfar)
    glm::mat4 Proj = glm::perspective (fov, ra, znear, zfar);
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform (glm::vec3 OBS, glm::vec3 VRP, glm::vec3 up) {
    // glm::lookAt (OBS, VRP, UP)
    glm::mat4 View = glm::lookAt (OBS, VRP, up);
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}


