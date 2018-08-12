/*
Nome: Eduardo da Silva Barbosa
DRE: 116150432
---------TAREFA 1--------------
*/

#include<stdlib.h>
#include<stdio.h>
#include<GL/gl.h>
#include<GL/freeglut.h>
#include<math.h>
#include <iostream>

using namespace std;
typedef struct _ponto{ /*Estrutura que define um ponto, com as coordenadas*/
    float coordX;
    float coordY;
} ponto;

/*Protótipos das funções utilizadas*/
void display();
void mouse(int button, int state, int X, int Y);
void keyboard_cb(unsigned char key, int X, int Y);
void suavizar();
bool colineares(int a, int b, int c);
ponto pontomedio(ponto A, ponto B);
ponto *resize(ponto *p, int Tamanho, int NovoTam);
void cabecalho();

/*Variáveis globais*/
int cont = 0; /*Cont é a quantidade de pontos existente*/
int tam = 100; /*Tamanho do array que vai guardar os pontos, inicialmente começa com 100 e cresce conforme a necessidade.*/
ponto *pontos = (ponto *) malloc (sizeof(ponto)*tam);/*Alocação de memória para o array de pontos*/

int main(int argc, char **argv){

  if(!pontos){ /*Verifico se a memória foi alocada para pontos*/
          printf("Erro: Nao foi possível alocar memória.\n");
          exit(1);
        }

    glutInit(&argc,argv); /*Esta função é utilizada para iniciar a biblioteca GLUT.*/
    /*glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);*/
    /*glutInitWindowPosition(200,200); *//*Posição da janela na tela*/
    glutInitWindowSize (1280, 720); /*Tamanho da Janela*/
    glutCreateWindow("Tarefa 1 - Eduardo da Silva Barbosa"); /*Nome da Janela*/
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
  /*glColor3f(1.0,1.0,1.0);*/
  glPointSize(8.0); /*Define o tamanho do ponto*/
  glLineWidth(6); /*Espessura da linha*/
  glBegin(GL_LINE_STRIP); /*GL_LINE_STRIP responsável por liga os pontos por uma linha reta*/
    for(int i = 0; i<cont; i++){
      glColor3f(1.0,1.0,1.0); /*Define a cor das linhas*/
      glVertex2f(pontos[i].coordX,pontos[i].coordY); /*Responsável por plotar os pontos*/
    }
  glEnd();
  glFlush();
  glutSwapBuffers();
}

void mouse(int button, int state, int X, int Y){
  if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
    if(cont >= tam){/*Verifica se existe espaço para o novo ponto*/
      pontos = resize(pontos,cont, tam*2);
    }
    pontos[cont].coordX = (float)(2.*X)/1280 - 1.; /*Necessário converter as coordenadas*/
    pontos[cont].coordY = (float)-(2.*Y)/720 + 1.; /*Necessário converter as coordenadas*/
    cont++;/*Contador de pontos existentes*/
    glutPostRedisplay();/*Atualiza o frame chamando novamente a função display*/
  }
}

void keyboard_cb(unsigned char key, int X, int Y){
  switch(key){
      case 27:         /*27 corresponde ao ESC, e está sendo utilizado para sair*/
          exit (0);
          free(pontos); /*Libera o espaço alocado para os pontos*/
          break;

      case 's': /*A tecla 's' está sendo utilizada para suavizar*/
          suavizar(); /*Chama a função responsável por fazer a suavização*/
          glutPostRedisplay();/*Atualiza o frame chamando novamente a função display*/
          break;
  }
}

void suavizar(){/*Função responsável por suavizar*/
  int j = 1;
  int a = 0;
  ponto *novospontos = (ponto *) malloc (sizeof(ponto)*cont*3); /*Aloca espaço para os novos pontos que serão descobertos*/

  if(!novospontos){ /*Verifico se a memória foi alocada para os novos pontos*/
          printf("Erro: Nao foi possível alocar memória.\n");
          exit(1);
        }
  ponto temp; /*É criada para guardar o último ponto*/

  for(int i = 0; i<cont-1; i++){
    if(!colineares(i,i+1,i+2)){/*só particiono a reta caso exista uma quina próxima, para isso verifíco se os pontos são colineares*/
      /*Encontro os novos pontos fazendo o ponto médio*/
      novospontos[a + 1] = pontomedio(pontos[i],pontos[i+1]);
      novospontos[a] = pontomedio(novospontos[a + 1],pontos[i]);
      novospontos[a + 2] = pontomedio(novospontos[a + 1],pontos[i+1]);
      a = a + 3;
    }
  }

  /*Verifico se 'pontos' terá capacidade para suportar os novos pontos, caso não tenha, é feito um resize*/
  if(a+cont>=tam){
    pontos = resize(pontos,cont,tam+(a*3));
  }

  /*Pego o último ponto*/
  temp.coordX = pontos[cont-1].coordX;
  temp.coordY = pontos[cont-1].coordY;

  /*Adiciono os pontos descobertos em 'Pontos'*/
  for(int k = 0; k<a; k++){
    pontos[j].coordX = novospontos[k].coordX;
    pontos[j].coordY = novospontos[k].coordY;
    j++;
  }

  /*Adiciono o último ponto*/
  pontos[j].coordX = temp.coordX;
  pontos[j].coordY = temp.coordY;
  cont = j+1;/*Atualizo o total de pontos existentes*/

  free(novospontos);/*Libero o espaço alocado para guardar os novos pontos, uma vez que os mesmos já foram jogados em 'Pontos'*/
/*
  printf("%d\n", cont);
  printf("%d\n", tam);*/
}

bool colineares(int a, int b, int c){ /*Verifico se 3 pontos consecutivos são colineares*/
  float determinante, temp1, temp2;
  float matrix[3][3];

  for(int i =0; i<3; i++){
    matrix[i][2] = 1;
  }

  /*Monto a matriz com cada coordenada dos pontos*/
  matrix[0][0] = pontos[a].coordX;
  matrix[0][1] = pontos[a].coordY;
  matrix[1][0] = pontos[b].coordX;
  matrix[1][1] = pontos[b].coordY;
  matrix[2][0] = pontos[c].coordX;
  matrix[2][1] = pontos[c].coordY;

  /*Calculo o determinante e verifico se é igual a 0 ou não*/
  temp1 =  matrix[0][0]*matrix[1][1]*matrix[2][2] + matrix[0][1]*matrix[1][2]*matrix[2][0] + matrix[1][0]*matrix[2][1]*matrix[2][2];
  temp2 =  matrix[0][2]*matrix[1][1]*matrix[2][0] + matrix[0][0]*matrix[1][2]*matrix[2][1] + matrix[1][0]*matrix[0][1]*matrix[2][2];
  determinante = temp1 - temp2;

  return (determinante == 0) ? 1 : 0;
}

ponto pontomedio(ponto A, ponto B){/*Função responsável por calcular o pont médio*/
  ponto pontoM;

  pontoM.coordX = (float)(A.coordX + B.coordX)/2; /*Ponto médio da coordenada X*/
  pontoM.coordY = (float)(A.coordY + B.coordY)/2; /*Ponto médio da coordenada Y*/

  return pontoM;
}

ponto *resize(ponto *p, int Tamanho, int NovoTam){ /*Função responsável por redimensionar o tamanho na memória para os pontos*/
  ponto *temp;
  temp = p;
  p = (ponto *) malloc (sizeof(ponto)*NovoTam);/*Aloca o novo espaço com o dobro da capacidade anterior */

  for(int i = 0; i<Tamanho; i++){/*Passa os pontos existentes para o novo espaço alocado*/
    p[i].coordX = temp[i].coordX;
    p[i].coordY = temp[i].coordY;
  }

  tam = NovoTam; /*Atualiza a nova capacidade de pontos*/
  free(temp); /*Libera o espaço alocado*/

  return p; /*Retorna o endereço dos pontos com capacidade maior na memórai*/
}

void cabecalho(){/*Função responsável por renderizar o cabeçalho*/
  string trab = "  ----------------------------------------------------  TAREFA 1  ----------------------------------------------------";
  string info = "Nome: Eduardo da Silva Barbosa - DRE: 116150432";
  string comandos =  "Comandos: Botao Esquerdo do mouse desenha / Tecla 's' suaviza";
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
