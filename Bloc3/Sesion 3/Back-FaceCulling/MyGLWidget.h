// MyGLWidget.h
#include "BL3GLWidget.h"

class MyGLWidget : public BL3GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL3GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void iniMaterialTerra ();
    virtual void initializeGL ();
    virtual void pasarInfoFoco();
    virtual void modelTransformPatricio();



  private:
    int printOglError(const char file[], int line, const char func[]);

    glm::vec3 posicion_foco_escena = glm::vec3(1.0, 1.0, 1.0);
    glm::vec3 posicion_foco_camara = glm::vec3(1.0, 1.0, 1.0);
    glm::vec3 color_foco_escena;
    glm::vec3 color_foco_camara;

    GLuint pos_foco_escena_Loc, color_foco_escena_Loc;
    GLuint pos_foco_camara_Loc, color_foco_camara_Loc;

    glm::vec3 posicion_patricio = glm::vec3 (0., 0., 0.);

    bool foco_escena = true;
    bool foco_camara = true;
    bool back_face_culling = false;

};