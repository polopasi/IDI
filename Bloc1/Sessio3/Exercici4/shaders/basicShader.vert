#version 330 core

in vec3 vertex;
in vec3 color;
out vec3 fcolor;
uniform mat4 TG;
uniform float scale;

void main()  {
    gl_Position = TG*vec4 (vertex*scale, 1.0);
    fcolor = color;
}
