/*
Nome: Eduardo da Silva Barbosa
DRE: 116150432
---------TAREFA 5--------------
Objetivo: Gerar a curva de bezier, a partir de uma curva de controle em R3.
*/

/*
compilação: g++ -o Tarefa5 Tarefa5.cpp -lglut -lGLU -lGL -Wall -ansi
*/

/*Bibliotecas Utilizadas*/
#include<stdlib.h>
#include<stdio.h>
#include<GL/gl.h>
#include<GL/freeglut.h>
#include<math.h>
#include <iostream>
#include <time.h>
#define NInteracoes 100

using namespace std;

typedef struct _Coordenadas{ /*Estrutura que define um ponto ou um vetor, com as coordenadas 'x', 'y' e 'z'.*/
    float coordX;
    float coordY;
    float coordZ;
} Coordenadas;

/*Protótipos das funções utilizadas*/
void display();
void display_manual();
void mouse(int button, int state, int X, int Y);
void keyboard_cb(unsigned char key, int X, int Y);
void controle(int key, int X, int Y);
void inicializa_poligonal();
void encontra_Bezier();
Coordenadas encontra_novo_ponto(int interacao);
void cabecalho();

/*Variáveis globais*/
int tam = 6; /*Tamanho do array que vai guardar os pontos, inicialmente começa com 100 e cresce conforme a necessidade.*/
Coordenadas *pontos = (Coordenadas *) malloc (sizeof(Coordenadas)*tam);/*Alocação de memória para o array de pontos*/
Coordenadas *bezier;
int condicao = 0;


int main(int argc, char **argv){

  if(!pontos){ /*Verifica se a memória foi alocada para 'pontos'.*/
          printf("Erro: Nao foi possível alocar memória.\n");
          exit(1);
        }

  inicializa_poligonal();

  /*GLUT Window Initialization*/
  glutInit(&argc,argv);


  glutInitWindowSize (1280, 400);
  glutInitWindowPosition(400, 0);
  glutCreateWindow("Manual");
  glutDisplayFunc(display_manual);

  glutInitWindowSize (1280, 720);
  glutInitWindowPosition(400, 120);
  glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  glutCreateWindow("Tarefa 5 - Eduardo da Silva Barbosa");


  glutDisplayFunc(display); /*Função que será chamada quando houver a necessidade de atualizar a janela*/

  // Set up OpenGL lights
  GLfloat light0_ambient[] =  {0.1f, 0.1f, 0.3f, 1.0f};
  GLfloat light0_diffuse[] =  {0.6f, 0.6f, 1.0f, 1.0f};
  GLfloat light0_position[] = {1.0f, 1.0f, 1.0f, 0.0f};

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
  glEnable( GL_COLOR_MATERIAL ) ;
  glEnable( GL_AUTO_NORMAL ) ;


  glutMouseFunc(mouse);
  glutKeyboardFunc(keyboard_cb);
  glutSpecialFunc(controle);


  glEnable(GL_DEPTH_TEST);

  glutMainLoop(); /*Inicia o gerenciamento de eventos, aguardando que algum evento seja acionado.*/

  return EXIT_SUCCESS ;

}

void display_manual(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  cabecalho();
  glutSwapBuffers();

}

void display(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  glRotatef(0,0,0,1.0);

  glPointSize(8.0);
  glLineWidth(4);

  glBegin(GL_LINE_STRIP);
      /*Eixo X*/
      glColor3f(1.0,0.0,0.0);
      glVertex3f(-1.0,0.0,0.0);
      glVertex3f(1.0,0.0,0.0);
  glEnd();

  glBegin(GL_LINE_STRIP);
      /*Eixo Y*/
      glColor3f(1.0,0.0,0.0);
      glVertex3f(0.0,1.0,0.0);
      glVertex3f(0.0,-1.0,0.0);
  glEnd();

  glBegin(GL_LINE_STRIP);
      /*Eixo Z*/
      glColor3f(1.0,0.0,0.0);
      glVertex3f(0.0,0.0,1.0);
      glVertex3f(0.0,0.0,-1.0);
  glEnd();

  glColor3f(1.0,1.0,1.0);
  glBegin(GL_LINE_STRIP);

    for(int i = 0; i<tam; i++){
      glVertex3f(pontos[i].coordX,pontos[i].coordY,pontos[i].coordZ); /*Responsável por plotar os pontos*/
    }

  glEnd();

  if(condicao != 0){
    glColor3f(1.0,1.0,0.0);
    glBegin(GL_LINE_STRIP);

      for(int i = 0; i<(2+NInteracoes); i++){
        glVertex3f(bezier[i].coordX,bezier[i].coordY,bezier[i].coordZ); /*Responsável a curva de bezier*/
      }
    glEnd();
  }

  glPopMatrix();
  glutSwapBuffers();
}

void mouse(int button, int state, int X, int Y){
}

void keyboard_cb(unsigned char key, int X, int Y){
  switch(key){
      case 27:         /*27 corresponde ao ESC, e está sendo utilizado para sair*/
          exit (0);
          free(pontos); /*Libera o espaço alocado para os pontos*/
          break;
      case 'B':
          encontra_Bezier();
          glutPostRedisplay();
          break;
      case 'b':
          encontra_Bezier();
          glutPostRedisplay();
          break;
  }
}

void controle(int key, int X, int Y){
  switch(key){
      case GLUT_KEY_UP :                  // GLUT_KEY_UP y GLUT_KEY_DOWN para control de rotacion general vertical
          glRotatef(-2.0, 2.0, 0.0, 0.0);
          break;

      case GLUT_KEY_DOWN :
          glRotatef(2.0, 2.0, 0.0, 0.0);
          break;

    case GLUT_KEY_LEFT :                // GLUT_KEY_LEFT y GLUT_KEY_RIGHT para control de rotacion general horizontal
          glRotatef(-2.0, 0.0, 2.0, 0.0);
          break;

    case GLUT_KEY_RIGHT :
          glRotatef(2.0, 0.0, 2.0, 0.0);
          break;
  }
  glutPostRedisplay();
}

void inicializa_poligonal(){
  /*Caso seja alterada a entrada, deve ser atualizado valor de tam (linha 42)*/
/*
  int i;
  float z = 0.05, t = 0.258;
  for(i = 0; i < tam; i++){
    pontos[i].coordX = (float) cos(t*i*2)/2;
    pontos[i].coordY = (float) z * i;
    pontos[i].coordZ = (float) sin(i*t*2)/2;
  }
*/
/*
  pontos[0].coordX = -0.5;
  pontos[0].coordY = 0.0;
  pontos[0].coordZ = 0.0;

  pontos[1].coordX = -0.3;
  pontos[1].coordY = 0.5;
  pontos[1].coordZ = -0.5;

  pontos[2].coordX = 0.3;
  pontos[2].coordY = 0.5;
  pontos[2].coordZ = 0.5;

  pontos[3].coordX = 0.5;
  pontos[3].coordY = 0.0;
  pontos[3].coordZ = 0.0;
*/

  pontos[0].coordX = -0.5;
  pontos[0].coordY = 0.0;
  pontos[0].coordZ = 0.0;

  pontos[1].coordX = -0.3;
  pontos[1].coordY = 0.5;
  pontos[1].coordZ = -0.5;

  pontos[2].coordX = 0.0;
  pontos[2].coordY = 0.5;
  pontos[2].coordZ = 0.0;

  pontos[3].coordX = 0.0;
  pontos[3].coordY = -0.5;
  pontos[3].coordZ = 0.5;

  pontos[4].coordX = 0.3;
  pontos[4].coordY = -0.5;
  pontos[4].coordZ = 0.5;

  pontos[5].coordX = 0.5;
  pontos[5].coordY = 0.0;
  pontos[5].coordZ = 0.0;


}

void encontra_Bezier(){
  int i;
  bezier = (Coordenadas *) malloc (sizeof(Coordenadas)*(2+NInteracoes));
  bezier[0] = pontos[0];
  bezier[NInteracoes+1] = pontos[tam-1];

  for(i = 1; i < NInteracoes+1; i++){
    bezier[i] = encontra_novo_ponto(i);
  }
  condicao = 1;
}

Coordenadas encontra_novo_ponto(int interacao){
  int i, j, aux;
  float t;
  Coordenadas temp[tam-1];
  j=0;
  aux = tam-1;

  for(i=0; i<tam; i++){/*copia pontos para temp*/
    temp[i].coordX = pontos[i].coordX;
    temp[i].coordY = pontos[i].coordY;
    temp[i].coordZ = pontos[i].coordZ;
  }

  t = (1.0/NInteracoes)*(interacao);

  while(aux != 0){/*Acha total de interações*/
    for(j=0; j < aux; j++){
      temp[j].coordX = ((1-t)*temp[j].coordX) + (t*temp[j+1].coordX);
      temp[j].coordY = ((1-t)*temp[j].coordY) + (t*temp[j+1].coordY);
      temp[j].coordZ = ((1-t)*temp[j].coordZ) + (t*temp[j+1].coordZ);
    }
    aux--;
  }

  return temp[0];
}

void cabecalho(){
  string trab = "  ----------------------------------------------------  TAREFA 5  ----------------------------------------------------";
  string info = "Nome: Eduardo da Silva Barbosa - DRE: 116150432";
  string comandos =  "Comandos: Botao Esquerdo do mouse: Desenha Pontos || Tecla B: Gera a curva de Bezier || Setas para girar";
  string linha = "______________________________________________________________________________________________________________________________________";

  int len1 = (int)trab.length();
  int len2 = (int)info.length();
  int len3 = (int)comandos.length();
  int len4 = (int)linha.length();

  glColor3f(1.0,1.0,1.0);

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
