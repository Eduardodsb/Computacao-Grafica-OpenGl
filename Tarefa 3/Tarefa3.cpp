/*
Nome: Eduardo da Silva Barbosa
DRE: 116150432
---------TAREFA 3--------------
*/

/*
compilação: g++ -o Tarefa3 Tarefa3.cpp -lglut -lGLU -lGL
*/

/*Bibliotecas Utilizadas*/
#include<stdlib.h>
#include<stdio.h>
#include<GL/gl.h>
#include<GL/freeglut.h>
#include<math.h>
#include <iostream>

using namespace std;

typedef struct _Coordenadas{ /*Estrutura que define um ponto ou um vetor, com as coordenadas 'x' e 'y'.*/
    float coordX;
    float coordY;
} Coordenadas;

/*Protótipos das funções utilizadas*/
void display();
void mouse(int button, int state, int X, int Y);
void keyboard_cb(unsigned char key, int X, int Y);
Coordenadas *resize(Coordenadas *p, int Tamanho, int NovoTam);
void cabecalho();
void fechoConvexo();
Coordenadas pontoExtremo();
float angulo_entre_vetores(Coordenadas vetor1, Coordenadas vetor2);
Coordenadas acha_vetor(Coordenadas origem, Coordenadas destino);
bool contem_ponto(Coordenadas *array, int tam, Coordenadas pontos);
float produto_interno(Coordenadas vetor1, Coordenadas vetor2);
float produto_vetorial(Coordenadas vetor1, Coordenadas vetor2);
float modulo_vetor(Coordenadas vetor);
bool compara_pontos(Coordenadas ponto1, Coordenadas ponto2);

/*Variáveis globais*/
int cont = 0; /*Cont é a quantidade de pontos existentes*/
int contp;/*Contp é a quantidade de pontos existentes no fecho convexo*/
int tam = 100; /*Tamanho do array que vai guardar os pontos, inicialmente começa com 100 e cresce conforme a necessidade.*/
Coordenadas *pontos = (Coordenadas *) malloc (sizeof(Coordenadas)*tam);/*Alocação de memória para o array de pontos*/
Coordenadas *pontosP; /*pontos pertecentes ao fecho convexo*/

int main(int argc, char **argv){

  if(!pontos){ /*Verifica se a memória foi alocada para 'pontos'.*/
          printf("Erro: Nao foi possível alocar memória.\n");
          exit(1);
        }

    glutInit(&argc,argv); /*Esta função é utilizada para iniciar a biblioteca GLUT.*/
    /*glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);*/
    /*glutInitWindowPosition(200,200); *//*Posição da janela na tela*/
    glutInitWindowSize (1280, 720); /*Tamanho da Janela*/
    glutCreateWindow("Tarefa 3 - Eduardo da Silva Barbosa"); /*Nome da Janela*/
    glutKeyboardFunc(keyboard_cb);/*Função que será chamada quando ocorrer um evento no teclado*/
    glutMouseFunc(mouse);/*Função que será chamada quando ocorrer um evento no mouse*/
    glutDisplayFunc(display); /*Função que será chamada quando houver a necessidade de atualizar a janela*/
    glutMainLoop(); /*Inicia o gerenciamento de eventos, aguardando que algum evento seja acionado.*/
    return EXIT_SUCCESS ;

}

void display(){/*Função responsável por plotar na tela*/
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  cabecalho();
  glPointSize(8.0); /*Define o tamanho do ponto*/
  glLineWidth(4); /*Define a espessura da linha*/

  glBegin(GL_POINTS); /*GL_POINTS responsável por desenhar os pontos*/
    glColor3f(1.0,1.0,1.0);/*Define a cor dos pontos */
    for(int i = 0; i<cont; i++){
      glVertex2f(pontos[i].coordX,pontos[i].coordY); /*Responsável por plotar os pontos*/
      printf("Ponto %d (coordX = %f , coordY = %f)\n",i,pontos[i].coordX, pontos[i].coordY);
    }
    printf("\n");
  glEnd();

  glBegin(GL_LINE_STRIP);/*Plota as linhas do fecho convexo*/
  glColor3f(1.0,1.0,1.0);/*Define a cor dos pontos */
  for(int i = 0; i<contp; i++){
    glVertex2f(pontosP[i].coordX,pontosP[i].coordY); /*Responsável por plotar os pontos*/
  }
  glEnd();

  glFlush();
  glutSwapBuffers();
}

void mouse(int button, int state, int X, int Y){
  if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
    if(cont >= tam){/*Verifica se existe espaço para o novo ponto*/
      pontos = resize(pontos,cont,tam*2);/*Aumenta a capacidade de pontos*/
      tam = tam*2; /*Atualiza a nova capacidade de pontos*/
    }
    pontos[cont].coordX = (float)(2.*X)/1280 - 1; /*Necessário converter as coordenadas*/
    pontos[cont].coordY = (float)-(2.*Y)/720 + 1; /*Necessário converter as coordenadas*/
    cont++;/*Contador de pontos existentes*/
    glutPostRedisplay();/*Atualiza o frame chamando novamente a função display*/
  }
}

void keyboard_cb(unsigned char key, int X, int Y){
  switch(key){
      case 27:         /*27 corresponde ao ESC, e está sendo utilizado para sair*/
          exit (0);
          free(pontos); /*Libera o espaço alocado para os pontos*/
          free(pontosP); /*Libera o espaço alocado para os pontos do polígono*/
          break;
      case 'f':
          fechoConvexo();
          glutPostRedisplay();
          break;
      case 'F':
          fechoConvexo();
          glutPostRedisplay();
          break;
  }
}

Coordenadas *resize(Coordenadas *p, int Tamanho, int NovoTam){ /*Função responsável por redimensionar o tamanho na memória para os pontos*/
  Coordenadas *temp;
  temp = p;
  p = (Coordenadas *) malloc (sizeof(Coordenadas)*NovoTam);/*Aloca o novo espaço com o dobro da capacidade anterior */

  for(int i = 0; i<Tamanho; i++){/*Passa os pontos existentes para o novo espaço alocado*/
    p[i].coordX = temp[i].coordX;
    p[i].coordY = temp[i].coordY;
  }
  free(temp); /*Libera o espaço alocado*/

  return p; /*Retorna o endereço dos pontos com capacidade maior na memórai*/
}

void cabecalho(){/*Função responsável por renderizar o cabeçalho*/
  string trab = "  ----------------------------------------------------  TAREFA 3  ----------------------------------------------------";
  string info = "Nome: Eduardo da Silva Barbosa - DRE: 116150432";
  string comandos =  "Comandos: Botao Esquerdo do mouse: Desenha Pontos || Tecla F: Gera o fecho convexo";
  string linha = "______________________________________________________________________________________________________________________________________";

  int len1 = (int)trab.length();
  int len2 = (int)info.length();
  int len3 = (int)comandos.length();
  int len4 = (int)linha.length();

  glRasterPos2f(-1.0,0.9);/*Posição onde será renderizado da próxima vez que a função glutBitmapCharacter()*/
  for (int i = 0; i < len1; i++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,trab[i]); /*renderiza o conteúdo da string passada.*/
  }

  glRasterPos2f(-1.0,0.8);
  for (int i = 0; i < len2; i++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,info[i]);
  }

  glRasterPos2f(-1.0,0.7);
  for (int i = 0; i < len3; i++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,comandos[i]);
  }

  glRasterPos2f(-1.0,0.68);
  for (int i = 0; i < len4; i++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,linha[i]);
  }
}

void fechoConvexo(){ /*Encontra o Fecho Convexo de uma nuvem de pontos*/
  int aux = 0;
  contp = 1;
  Coordenadas pontoTemp, semireta, vetor1;
  pontosP = (Coordenadas *) malloc (sizeof(Coordenadas)*(cont+1)); /*Guardará os pontos pertencente ao fecho convexo*/
  if(!pontosP){ /*Verifica se a memória foi alocada para 'pontosP'.*/
          printf("Erro: Nao foi possível alocar memória.\n");
          exit(1);
  }

  pontosP[0] = pontoExtremo();

  pontoTemp.coordX = pontosP[0].coordX;
  pontoTemp.coordY = pontosP[0].coordY;
  semireta.coordX = 1; /*Semireta é o ponto utilizado para gerar o vetor pertecente a SemiReta base a partir da qual será verificado os angulos*/
  semireta.coordY =  pontosP[0].coordY;

  vetor1 = acha_vetor(pontoTemp,semireta); /*Vetor1 é o vetor pertecente a SemiReta*/

  do{
    for(int j = 0; j<cont; j++){/*A partir de um vetor vai comparar o ângulo com todos os outros*/
      if(!compara_pontos(pontoTemp,pontos[j]) && (produto_vetorial(vetor1,acha_vetor(pontoTemp,pontos[j]))>0.0) && (!contem_ponto(pontosP,contp,pontos[j]) || compara_pontos(pontos[j],pontosP[0]))){ /*Não é permitodo pontos que já estejam em pontosP (com exceção do ponto mais extremo que está em pontosP[0]). Não é permitido pontos que possuem produto vetorial menor que 0. E não é permitido pegar o mesmo ponto*/
        if(aux == 0){/*Caso inicial para ser possível fazer a primeira comparação é necessário ter um possível menor angulo em pontosP*/
          pontosP[contp].coordX = pontos[j].coordX;
          pontosP[contp].coordY = pontos[j].coordY;
          aux++;
        }
        if(angulo_entre_vetores(vetor1,acha_vetor(pontoTemp,pontos[j])) < (angulo_entre_vetores(vetor1,acha_vetor(pontoTemp,pontosP[contp])))){/*Compara em busca de um vetor com o ângulo menor*/
          pontosP[contp].coordX = pontos[j].coordX;
          pontosP[contp].coordY = pontos[j].coordY;
        }
      }
    }
    pontoTemp.coordX = pontosP[contp].coordX;/*Define o próximo ponto, ao qual será gerado um vetor da SemiReta, e tal vetor será comparado com todos os outros em busca do próximo com menor ângulo (Com produto vetorial positivo)*/
    pontoTemp.coordY = pontosP[contp].coordY;
    semireta.coordX = pontosP[contp-1].coordX;
    semireta.coordY =  pontosP[contp-1].coordY;
    vetor1 = acha_vetor(semireta,pontoTemp);/*Define o vetor da nova SemiReta*/
    contp++;
    aux = 0;
  }while((!compara_pontos(pontosP[contp-1],pontosP[0])));/*Deve para quando chegar no ponto mais extremo*/

}

Coordenadas pontoExtremo(){ /*Acha o ponto mais extremo (menor) em relação ao Y*/
  Coordenadas ponto_extremo;
  int i;
  ponto_extremo.coordX = pontos[0].coordX;
  ponto_extremo.coordY = pontos[0].coordY;
  for(i = 0; i<cont; i++){
    if(ponto_extremo.coordY>pontos[i].coordY){
      ponto_extremo.coordX = pontos[i].coordX;
      ponto_extremo.coordY = pontos[i].coordY;
    }
  }
  return ponto_extremo;
}

Coordenadas acha_vetor(Coordenadas origem, Coordenadas destino){ /*Acha o vetor com a origem em (0,0)*/
  Coordenadas vetor;
  vetor.coordX = destino.coordX - origem.coordX;
  vetor.coordY = destino.coordY - origem.coordY;
  return vetor;
}

float angulo_entre_vetores(Coordenadas vetor1, Coordenadas vetor2){ /*Encontra o angulo entre dois vetores com a mesma origem*/
    return acos(produto_interno(vetor1,vetor2)/(modulo_vetor(vetor1)*modulo_vetor(vetor2)));
}

bool contem_ponto(Coordenadas *array, int tam, Coordenadas ponto){ /*Verifica se um ponto específico está contido em um array de pontos*/
  for(int i = 0; i<tam; i++){
    if((array[i].coordX == ponto.coordX) && (array[i].coordY == ponto.coordY)){
      return 1;
    }
  }
  return 0;
}

float produto_interno(Coordenadas vetor1, Coordenadas vetor2){ /*Produto interno com dois vetores de mesma origem*/
  return (vetor1.coordX*vetor2.coordX) + (vetor1.coordY*vetor2.coordY);
}

float produto_vetorial(Coordenadas vetor1, Coordenadas vetor2){ /*Produto vetorial com dois vetores de mesma origem*/
  return (vetor1.coordX*vetor2.coordY) - (vetor1.coordY*vetor2.coordX); /*Componente Z do produto vetorial, uma vez que as componentes entre X e Y são zero.*/
}

float modulo_vetor(Coordenadas vetor){ /*Módulo de um vetor*/
  return sqrt(pow(vetor.coordX,2)+pow(vetor.coordY,2));
}

bool compara_pontos(Coordenadas ponto1, Coordenadas ponto2){ /*Verifica se dois pontos são iguais*/
  if((ponto1.coordX == ponto2.coordX) && (ponto1.coordY == ponto2.coordY)){
    return 1;
  }else{
    return 0;
  }
}
