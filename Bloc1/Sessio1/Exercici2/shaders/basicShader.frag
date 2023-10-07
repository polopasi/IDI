#version 330 core

in vec3 fcolor;
out vec4 FragColor;

void main() {
    //FragColor = vec4(fcolor, 1);

    int x = int (gl_FragCoord.y/50)%2;

    if (x == 0) {

        if (gl_FragCoord.x < 354. && gl_FragCoord.y > 354.)
            FragColor = vec4(1.0, 0.0, 0.0, 1.0);
        else if (gl_FragCoord.x > 354. && gl_FragCoord.y > 354.)
            FragColor = vec4(0.0, 0.0, 1.0, 1.0);
        if (gl_FragCoord.x < 354. && gl_FragCoord.y < 354.)
            FragColor = vec4(1.0, 1.0, 0.0, 1.0);
        else if (gl_FragCoord.x > 354. && gl_FragCoord.y < 354.)
            FragColor = vec4(0.0, 1.0, 0.0, 1.0);

    }
    else discard;

}

