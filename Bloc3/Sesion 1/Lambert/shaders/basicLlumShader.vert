#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;

// Valors per als components que necessitem del focus de llum
vec3 colorFocus = vec3(0.8, 0.8, 0.8);
vec3 llumAmbient = vec3(0.2, 0.2, 0.2);
vec3 posFocus = vec3(1, 1, 1);  // en SCA

out vec3 fcolor;

vec3 Ambient() {
    return llumAmbient * matamb;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus) 
{
    // Tant sols retorna el terme difús
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
    vec3 colRes = vec3(0);
    // Càlcul component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colFocus * matdiff * dot (L, NormSCO);
    return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec4 vertSCO, vec3 colFocus) 
{
    // Tant sols retorna el terme especular!
    // Els vectors rebuts com a paràmetres (NormSCO i L) estan normalitzats
    vec3 colRes = vec3 (0);
    // Si la llum ve de darrera o el material és mate no fem res
    if ((dot(NormSCO,L) < 0) || (matshin == 0))
      return colRes;  // no hi ha component especular

    // Calculem R i V
    vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
    vec3 V = normalize(-vertSCO.xyz); // perquè la càmera està a (0,0,0) en SCO

    if (dot(R, V) < 0)
      return colRes;  // no hi ha component especular
    
    // Calculem i retornem la component especular
    float shine = pow(max(0.0, dot(R, V)), matshin);
    return (matspec * colFocus * shine); 
}

void main()
{	
    
    //La solucion es fcolor = Ambiente() + Difus(...) + Especular (...)
    /*
     * Calculamos los parametros de la luz difusa:
     * normalSCO = NM * Normal donde NM = inversa de la traspuesta de View*TG
     * para conseguir el vector normal en SCO es necesario hacer
     * normalMatrix = inverse (transpose (mat3 (view * TG))) que retorna un mat3,
     * y multiplicarlo por normal. normSCO = (inversa de la traspuesta de View*TG)*normal.
     *
     * L = posFocusSCO - verticesSCO
     * Para calcular el vector L, hemos de pasar la posicion del foco a SCO, como originalmente
     * esta en SCA, solo hemos de multiplicar por view: L = view*posFocus. ¡Cuidado! Como View
     * es un vec4, hemos de calcular posFocusSCO como vec4, pero L es un vec3:
     * Para escoger las tres coordenadas de L (convertirlo en vec3) podemos hacer:
     * L = posFocusSCO.xyz
     * Para calcular los verticesSCO, y estan en SCM, hay que multiplicarlos view*TG*vertices:
     * vec4 vertexSCO = view*TG*vec4 (vertex, 1.0);
     * Finalmente hay que restar:
     * L = posFocusSCO.xyz - vertexSCO.xyz;
    */

    mat3 normalMatrix = inverse (transpose (mat3 (view * TG)));
    vec3 normSCO = normalMatrix * normal;

    vec4 posFocusSCO = view * vec4(posFocus, 1);
    vec4 vertexSCO = view * TG * vec4 (vertex, 1.0);
    vec3 L = posFocusSCO.xyz - vertexSCO.xyz;

    //ATENCION: Hay que normalizar los vectores N y L con la funcion normalize.
    fcolor = Ambient() + Difus (normalize(normSCO), normalize(L), colorFocus);
    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}
