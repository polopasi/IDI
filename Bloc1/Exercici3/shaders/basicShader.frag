#version 330 core

out vec4 FragColor;

void main() {
    //FragColor = vec4(0, 0, 0, 1);
    if (gl_FragCoord.x < 354. && gl_FragCoord.y > 354.)
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    else if (gl_FragCoord.x > 354. && gl_FragCoord.y > 354.)
        FragColor = vec4(0.0, 0.0, 1.0, 1.0);
    if (gl_FragCoord.x < 354. && gl_FragCoord.y < 354.)
        FragColor = vec4(1.0, 1.0, 0.0, 1.0);
    else if (gl_FragCoord.x > 354. && gl_FragCoord.y < 354.)
        FragColor = vec4(0.0, 1.0, 0.0, 1.0);

}

