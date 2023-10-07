#version 330 core

in vec3 fcolor;
out vec4 FragColor;

void main() {

    int x = int (gl_FragCoord.y/50)%2;

    if (x == 0) FragColor = vec4(fcolor, 1);
    else discard;

}

