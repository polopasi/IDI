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

void MyGLWidget::initializeGL ()
{
  BL3GLWidget::initializeGL();
  //"Enlazo" los uniforms del vertex Shader a mis GLuint.
  pos_foco_Loc = glGetUniformLocation (program->programId(), "posFocus");
  color_foco_Loc = glGetUniformLocation (program->programId(), "colorFocus");
  pasarInfoFoco();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_K: 
      if (posicion_foco.x > -5) 
        posicion_foco = glm::vec3 (posicion_foco.x - 0.25, posicion_foco.y, posicion_foco.z);
      break;
  case Qt::Key_L: 
      if (posicion_foco.x < 5) 
        posicion_foco = glm::vec3 (posicion_foco.x + 0.25, posicion_foco.y, posicion_foco.z);
      break;
    
    default: BL3GLWidget::keyPressEvent(event); break;
  }
  pasarInfoFoco();
  update();
}

void MyGLWidget::iniMaterialTerra ()
{
  //Le doy un valor plastico brillante azul. (No tiene luz especular, unicamente ambiente y difusa)
  amb = glm::vec3(0.0,0.0,0.8);
  diff = glm::vec3(0,0,0.8);
  spec = glm::vec3(0,0,0);
  shin = 100;
}

void MyGLWidget::pasarInfoFoco()
{

  glUniform3fv(pos_foco_Loc, 1, &posicion_foco[0]);
  
  color_foco = glm::vec3(0.8, 0.8, 0.8);
  glUniform3fv(color_foco_Loc, 1, &color_foco[0]);
}