#version 330 core

in vec3 vertex;
in vec3 color;

uniform mat4 TG;
uniform mat4 proj;      //Matriz de la cámara de proyección
uniform mat4 view;      //Matriz view de la cámara de proyección.


out vec3 fcolor;

void main()  {
    fcolor = color;
    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}
