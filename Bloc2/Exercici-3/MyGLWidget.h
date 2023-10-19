// MyGLWidget.h
#include "BL2GLWidget.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

    virtual void initializeGL ();

    virtual void carregaShaders();

    void projectTransform(float fov, float ra, float znear, float zfar);

    void viewTransform(glm::vec3 OBS, glm::vec3 VRP, glm::vec3 up);

    void ini_camera (glm::vec3 OBS, glm::vec3 VRP, glm::vec3 up, float FOV, 
                            float ra, float znear, float zfar);

  private:
    GLuint projLoc;
    GLuint viewLoc;
    int printOglError(const char file[], int line, const char func[]);
};
