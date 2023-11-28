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

void MyGLWidget::ini_camera () {
    raw = float (width()/height());
    projectTransform();
    viewTransform();
}

void MyGLWidget::resizeGL (int w, int h) {

    rav = float (w) / float (h);
    
    raw = rav;
    if (rav < 1) FOV = 2*atan (tan (FOVoptimo/2.0)/rav);
    projectTransform();
}


void MyGLWidget::initializeGL () {
    BL2GLWidget::initializeGL();
    glEnable (GL_DEPTH_TEST);   //Activar el z-buffer.
    calcularCajaPatricio();
    calcularCajaEscena();

    psi = theta = 0;
    //projectTransform() y viewTransform() solo 
    //se ha de llamar una vez, en initializeGL ya va bien.
    ini_camera();       
    //CUIDADO: se ha de llamar DESPUÉS de carregaShaders, sino da core dumped.
    creaBuffers();
} 

void MyGLWidget::paintGL () {
    // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
    // useu els paràmetres que considereu (els que hi ha són els de per defecte)
    //  glViewport (0, 0, ample, alt);
  
    // Esborrem el frame-buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Pintamos el modelo
    calcularCajaPatricio();
    modelTransform ();
    glBindVertexArray (VAO_Model);
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

void MyGLWidget::projectTransform () {
    // glm::perspective (FOV en radianes, ra window, znear, zfar)
    glm::mat4 Proj = glm::perspective (FOV, raw, distancia-radio,distancia+radio);
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform () {

    //glm::mat4 View = glm::lookAt (obsEscena, centroEscena, up);

    glm::mat4 View (1.0);
    View = glm::translate (View, glm::vec3(0.,0.,-distancia));
    View = glm::rotate (View, theta, glm::vec3 (1., 0., 0.));
    View = glm::rotate (View, -psi, glm::vec3 (0., 1., 0.));
    View = glm::translate (View, -centroEscena);
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::creaBuffers () {

  //MODELOS 3D OBJ
  //m es una variable de clase!
  m.load ("./models/Patricio.obj");

  // Creación del VAO para pintar.
  glGenVertexArrays(1, &VAO_Model);
  glBindVertexArray(VAO_Model);
  
  GLuint VBO_Model_posicion;
  glGenBuffers(1, &VBO_Model_posicion);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model_posicion);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, 
				m.VBO_vertices(), GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  GLuint VBO_Model_color;
  glGenBuffers(1, &VBO_Model_color);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model_color);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, 
				m.VBO_matdiff(), GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  //-------- Suelo ------------


  glm::vec3 suelo_posicion[6] = {

    glm::vec3(2.0, 0.0, 2.0),
    glm::vec3(2.0, 0.0, -2.0),
    glm::vec3(-2.0, 0.0, -2.0),
    
    glm::vec3(2.0, 0.0, 2.0),
    glm::vec3(-2.0, 0.0, -2.0),
    glm::vec3(-2.0, 0.0, 2.0)
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
  transform = glm::scale(transform, glm::vec3(escalaPatricio,escalaPatricio,escalaPatricio));
  transform = glm::translate(transform, -centroBasePatricio);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}


void MyGLWidget::modelTransformSuelo () 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform, glm::vec3(0, 0, 0));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::calcularCajaEscena () {


  puntoMinimoEscena = glm::vec3(-2.5, 0, -2.5);
  puntoMaximoEscena = glm::vec3(+2.5, puntoMaximoEscena.y, +2.5);

  centroEscena = (puntoMaximoEscena + puntoMinimoEscena);
  centroEscena /= 2;

  radio = length (puntoMaximoEscena - puntoMinimoEscena)/2;
  distancia = 2*radio;

  distanciaEscena =  (puntoMaximoEscena - puntoMinimoEscena);

  obsEscena = centroEscena + glm::vec3 (0, 0, distancia);

  FOVoptimo = FOV = glm::asin (radio/distancia)*2;
}

void MyGLWidget::calcularCajaPatricio () {
  float minx, maxx, miny, maxy, minz, maxz;

  minx = maxx = m.vertices()[0];
  miny = maxy = m.vertices()[1];
  minz = maxz = m.vertices()[2];


  //Encuentro los puntos máximos y mínimos del modelo.
  for (unsigned int i = 3; i < m.vertices().size(); i += 3) {
    if (m.vertices()[i] < minx) minx = m.vertices()[i];
    if (m.vertices()[i] > maxx) maxx = m.vertices()[i];

    if (m.vertices()[i + 1] < miny) miny = m.vertices()[i + 1];
    if (m.vertices()[i + 1] > maxy) maxy = m.vertices()[i + 1];

    if (m.vertices()[i + 2] < minz) minz = m.vertices()[i + 2];
    if (m.vertices()[i + 2] > maxz) maxz = m.vertices()[i + 2];
  }
  
  puntoMinimoEscena = glm::vec3(puntoMinimoEscena.x, miny, puntoMinimoEscena.z);
  puntoMaximoEscena = glm::vec3(puntoMaximoEscena.x, 4, puntoMaximoEscena.z);

  escalaPatricio = 4/(maxy - miny);
  puntoMedioPatricio = glm::vec3 ((maxx + minx)/2, (maxy + miny)/2, (maxz + minz)/2);
  centroBasePatricio = glm::vec3 ((maxx + minx)/2, miny, (maxz + minz)/2);

}
