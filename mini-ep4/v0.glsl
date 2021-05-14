// Digite aqui seu código GLSL para gerar uma imagem
// O GLSL é muito próximo do C, mas não é idêntico.

// Essa linha vai definir a precisão do float,
// e mediump é bom o bastante no momento.
precision mediump float;

// Aqui não tem conversão automática entre int e float
// Então coloque .0 quando precisar de floats

// As janelas tem um tamanho fixo de 3000x3000
#define WIDTH (3000.0)
#define HEIGHT (3000.0)

// Constantes do limite do plano cartesiano
#define Xi (-1.0)
#define Xj (+1.0)
#define Yi (-1.0)
#define Yj (+1.0)

#define Xd (Xj - Xi)
#define Yd (Yj - Yi)


// diferente do código em JS
// a GPU roda essa função main para cada pixel
void main() {
    // Aqui o pixel é guardado num vetor de 4 floats
    // Também segue o formato RGBA, mas dessa vez
    // os campos variam de 0.0 a 1.0

    // vamos obter as coordenadas do plano a partir da
    // variável gl_FragCoord que é pré definida para cada pixel
    vec2 xy = (gl_FragCoord.xy / vec2(WIDTH, HEIGHT) * vec2(Xd, Yd)) + vec2(Xi, Yi);

    // Para acessa o x ou o y individualmente é só usar
    // xy.x e xy.y respectivamente
    float x = xy.x;
    float y = xy.y;

    //****************************
    //compute a cor do pixel aqui

    // ex: pintar de acordo com a distância do centro
    // usaremos a função help para ajudar a computar a distância

    float distancia = (x*x + y*y)/2.0;

    // invertido para o meio ser branco
    float corrigido = 1.0 - distancia;

    // para dar uma "cor" podemos variar a presença de verde pelo y
    // e o de azul pelo x

    float fator_verde = (xy.y+1.0)/2.0;
    float fator_azul = (xy.x+1.0)/2.0;

    float red = corrigido;
    float green = corrigido * fator_verde;
    float blue = corrigido * fator_azul;

    //*****************************
    // Aplica a cor
    gl_FragColor = vec4(red, green, blue, 1.0);
}

// Clique em Run GLSL para rodar o código ;)