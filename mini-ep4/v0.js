// Digite aqui seu código JS para gerar uma imagem

// As janelas tem um tamanho fixo de 3000x3000
const WIDTH = 3000;
const HEIGHT = 3000;

// Constantes do limite o plano cartesiano
const Xi = -1.0;
const Xj = +1.0;
const Yi = -1.0;
const Yj = +1.0;

const Xd = Xj - Xi;
const Yd = Yj - Yi;

const main = (data) => {
    //Data é um array de bytes que representa a imagem
    //O valor mínimo é 0 e o máximo é 255
    //A cada 4 bytes temos um pixel
    //Red Green Blue Alpha

    //Vamos usar o for para percorrer cada pixel
    for(let i = 0; i < data.length; i += 4) {
        const _x = Math.floor((i/4) % 3000);
        const _y = Math.floor((i/4) / 3000);

        //converte para um plano cartesiano indo de Xi-Xj e Yi-Yj.
        const x = Xi + (_x/WIDTH) * Xd;
        const y = Yj - (_y/HEIGHT) * Yd;

        //****************************
        //compute a cor do pixel aqui

        // ex: pintar de acordo com a distância do centro
        // (a distância máxima para os Xi-Xj e Yi-Yj atuais é raiz de 2)
        // (essa distância não é distância em si, eu sei)

        const distancia = (x*x + y*y)/2;

        // transforma num número entre 0 e 255
        // invertido para o meio ser branco
        const corrigido = 255 - distancia*255;

        // para dar uma cor podemos variar a presença de verde pelo y
        // e o de azul pelo x

        const fator_verde = (y+1)/2;
        const fator_azul = (x+1)/2;

        // note que o próprio JS vai converter o número
        // para um inteiro entre 0 e 255 ;)

        const red = corrigido;
        const green = corrigido * fator_verde;
        const blue = corrigido * fator_azul;

        //*****************************
        
        //define a cor do pixel
        data[i]   = red;
        data[i+1] = green;
        data[i+2] = blue;
        data[i+3] = 255;
    }
};

// O nome da função a ser executada retornada deve aparecer no final
// clique em Run JS e aguarde um pouco para o seu código executar
main