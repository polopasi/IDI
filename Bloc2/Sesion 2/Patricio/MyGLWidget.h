// MyGLWidget.h
#include "BL2GLWidget.h"
#include "model.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

    virtual void initializeGL ();

    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);

    virtual void paintGL();

    virtual void carregaShaders();
    
    void projectTransform();

    void viewTransform();

    void ini_camera ();

    virtual void creaBuffers ();

    virtual void keyPressEvent(QKeyEvent* event);

    virtual void modelTransform ();

    void modelTransformSuelo ();

    void calcularCajaEscena ();

    void calcularCajaPatricio ();

    void calcularCajaSuelo ();

    


  private:
    float angle = 0;
    GLuint projLoc;
    GLuint viewLoc;
    GLuint VAO_Model;
    
    GLuint VAO_Suelo;
    GLuint VBO_Suelo_posicion;
    GLuint VBO_Suelo_color;


    glm::vec3 centroEscena = glm::vec3 (0, 0, 0);            //VRP = CentroEscena.
    glm::vec3 distanciaEscena = glm::vec3 (0,0,0);
    glm::vec3 obsEscena = glm::vec3 (0,0,0);
    glm::vec3 up = glm::vec3 (0, 1, 0);
    

    glm::vec3 centroBasePatricio, puntoMedioPatricio, centroBaseSuelo;
    float escalaPatricio, escalaSuelo;
    glm::vec3 puntoMaximoEscena, puntoMinimoEscena;

    float radio, distancia, FOVoptimo, FOV;
    GLuint w, h;
    float rav;                                          //Aspect ratio viewport
    float raw = width()/height();                           //Aspect ratio Window
    
    Model m; //m es el modelo
    int printOglError(const char file[], int line, const char func[]);
};
