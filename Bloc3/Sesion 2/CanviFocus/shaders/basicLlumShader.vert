#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

out vec3 matambF;
out vec3 matdiffF;
out vec3 matspecF;
out float matshinF;

out vec3 normSCOShader;
out vec4 vertexSCOShader;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;


void main()
{	
    mat3 normalMatrix = inverse (transpose (mat3 (view * TG)));
    vec3 normSCO = normalMatrix * normal;


    matambF = matamb;
    matdiffF = matdiff;
    matspecF = matspec;
    matshinF = matshin;

    //ATENCION: Hay que normalizar los vectores N y L con la funcion normalize.
    normSCOShader = normalize(normSCO);

    vec4 vertexSCO = view * TG * vec4 (vertex, 1.0);
    vertexSCOShader = vertexSCO;
    
    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}