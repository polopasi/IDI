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
    glEnable (GL_DEPTH_TEST);   //Activar el z-buffer.
    //projectTransform() y viewTransform() solo 
    //se ha de llamar una vez, en initializeGL ya va bien.
    ini_camera(glm::vec3(0,0,1), glm::vec3(0,0,0), glm::vec3(0,1,0), M_PI/2.0f,
                1.0, 0.4, 3);       
    //CUIDADO: se ha de llamar DESPUÉS de carregaShaders, sino da core dumped.
    creaBuffers();
} 

void MyGLWidget::paintGL () {
    // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
    // useu els paràmetres que considereu (els que hi ha són els de per defecte)
    //  glViewport (0, 0, ample, alt);
  
    // Esborrem el frame-buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Pintamos a Homer
    modelTransform ();
    glBindVertexArray (VAO_Homer);
    glDrawArrays (GL_TRIANGLES, 0, m.faces ().size () * 3);

    //Pintamos el suelo
    glBindVertexArray (VAO_Suelo);
    modelTransformSuelo();
    glDrawArrays (GL_TRIANGLES, 0, 6);

    glBindVertexArray (0);
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

void MyGLWidget::creaBuffers () {

  //MODELOS 3D OBJ
  //m es una variable de clase!
  m.load ("./models/HomerProves.obj");

  // Creación del VAO para pintar.
  glGenVertexArrays(1, &VAO_Homer);
  glBindVertexArray(VAO_Homer);
  
  GLuint VBO_Homer_posicion;
  glGenBuffers(1, &VBO_Homer_posicion);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer_posicion);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, 
				m.VBO_vertices(), GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  GLuint VBO_Homer_color;
  glGenBuffers(1, &VBO_Homer_color);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer_color);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, 
				m.VBO_matdiff(), GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  //-------- Suelo ------------


  glm::vec3 suelo_posicion[6] = {

	glm::vec3(2.0, -1.0, 2.0),
	glm::vec3(2.0, -1.0, -2.0),
	glm::vec3(-2.0, -1.0, -2.0),
	
	glm::vec3(2.0, -1.0, 2.0),
	glm::vec3(-2.0, -1.0, -2.0),
	glm::vec3(-2.0, -1.0, 2.0)
  };

    glm::vec3 suelo_color[6] = {
		glm::vec3(1,0,1),
		glm::vec3(1,0,1),
		glm::vec3(1,0,1),
		glm::vec3(1,0,1),
		glm::vec3(1,0,1),
		glm::vec3(1,0,1),
	};

  glGenVertexArrays (1, &VAO_Suelo);
  glBindVertexArray (VAO_Suelo);

  glGenBuffers(1, &VBO_Suelo_posicion);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Suelo_posicion);
  glBufferData(GL_ARRAY_BUFFER, sizeof(suelo_posicion), suelo_posicion, GL_STATIC_DRAW);
  //Activamos atributo vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);


  glGenBuffers(1, &VBO_Suelo_color);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Suelo_color);
  glBufferData(GL_ARRAY_BUFFER, sizeof(suelo_color), suelo_color, GL_STATIC_DRAW);
  //Activamos atributo vertexLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);  

  glBindVertexArray (0);

}


void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_R: { // escalar a més gran
      angle += M_PI/4.0f;
      break;
    }
    default: event->ignore(); break;
  }
  update();
}


void MyGLWidget::modelTransform () 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::rotate(transform, angle, glm::vec3(0,1,0));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}


void MyGLWidget::modelTransformSuelo () 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform, glm::vec3(0,0,0));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

