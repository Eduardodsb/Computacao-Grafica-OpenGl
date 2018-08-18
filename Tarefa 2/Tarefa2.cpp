/*
Nome: Eduardo da Silva Barbosa
DRE: 116150432
---------TAREFA 2--------------
*/

/*
compilação: g++ -o Tarefa2 Tarefa2.cpp -lglut -lGLU -lGL
*/

/*Bibliotecas Utilizadas*/
#include<stdlib.h>
#include<stdio.h>
#include<GL/gl.h>
#include<GL/freeglut.h>
#include<math.h>
#include <iostream>

using namespace std;

typedef struct _ponto{ /*Estrutura que define um ponto, com as coordenadas 'x' e 'y'.*/
    float coordX;
    float coordY;
} ponto;

typedef struct _vetor{ /*Estrutura que define um vetor, com os pontos inicial e final*/
  ponto p1; /*Origem do vetor*/
  ponto p2; /*Fim do vetor*/
} vetor;


/*Protótipos das funções utilizadas*/
void display();
void mouse(int button, int state, int X, int Y);
void keyboard_cb(unsigned char key, int X, int Y);
ponto *resize(ponto *p, int Tamanho, int NovoTam);
void cabecalho();
float distancia_entre_pontos(ponto p1, ponto p2);
void localizaP();
bool intersecao(vetor v1, vetor v2);
float produto_vetorial(ponto p1, ponto p2);

/*Variáveis globais*/
int cont = 0; /*Cont é a quantidade de pontos existentes*/
int tam = 100; /*Tamanho do array que vai guardar os pontos, inicialmente começa com 100 e cresce conforme a necessidade.*/
int etapa = 1; /*Para controlar a etapa 1 (Construir o pílogono) e a etapa 2 (Escolher um ponto)*/
ponto p; /*Ponto que será utilizado para descobrir a localização*/
ponto *pontos = (ponto *) malloc (sizeof(ponto)*tam);/*Alocação de memória para o array de pontos*/
string res = "Resultado:" ;/*Utilizado para renderizar o resultado */

int main(int argc, char **argv){

  if(!pontos){ /*Verifica se a memória foi alocada para 'pontos'.*/
          printf("Erro: Nao foi possível alocar memória.\n");
          exit(1);
        }

    glutInit(&argc,argv); /*Esta função é utilizada para iniciar a biblioteca GLUT.*/
    /*glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);*/
    /*glutInitWindowPosition(200,200); *//*Posição da janela na tela*/
    glutInitWindowSize (1280, 720); /*Tamanho da Janela*/
    glutCreateWindow("Tarefa 2 - Eduardo da Silva Barbosa"); /*Nome da Janela*/
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

  if(etapa == 2){/*Após a construção do polígono essa parte também será renderizada*/
    glBegin(GL_POINTS);/*Renderiza pontos*/
    glColor3f(1.0,0.0,0.0);/*Define a cor do ponto escolhido para vermelho*/
    glVertex2f(p.coordX,p.coordY);/*Renderiza o ponto escolhido*/
    glEnd();
  }

  glBegin(GL_LINE_STRIP); /*GL_LINE_STRIP responsável por liga os pontos por uma linha reta*/
    glColor3f(1.0,1.0,1.0);/*Define a cor das linhas*/
    for(int i = 0; i<cont-1; i++){
      glVertex2f(pontos[i].coordX,pontos[i].coordY); /*Responsável por plotar os pontos*/
    }
    if((cont>3) && (distancia_entre_pontos(pontos[0],pontos[cont-1]) < 0.05) && etapa == 1){ /*Verifica sempre se o último ponto escolhido está próximo do primeiro ponto do polígono*/
      glVertex2f(pontos[0].coordX,pontos[0].coordY);/*Caso esteja, conecta ao primeiro ponto do polígino*/
      etapa = 2; /*Ativa a etapa 2 e em seguida define o útimo ponto como sendo o primeiro*/
      pontos[cont-1].coordX = pontos[0].coordX;
      pontos[cont-1].coordY = pontos[0].coordY;
    }else if(cont > 1){
      glVertex2f(pontos[cont-1].coordX,pontos[cont-1].coordY); /*Responável por plotar o último ponto*/
    }
  glEnd();
  glFlush();
  glutSwapBuffers();
}

void mouse(int button, int state, int X, int Y){
  if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && etapa == 1){
    if(cont >= tam){/*Verifica se existe espaço para o novo ponto*/
      pontos = resize(pontos,cont, tam*2);
    }
    pontos[cont].coordX = (float)(2.*X)/1280 - 1; /*Necessário converter as coordenadas*/
    pontos[cont].coordY = (float)-(2.*Y)/720 + 1; /*Necessário converter as coordenadas*/
    cont++;/*Contador de pontos existentes*/
    glutPostRedisplay();/*Atualiza o frame chamando novamente a função display*/
  }
  if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && etapa == 2){/*Caso a segunda etapa esteja ativa, o botão do mouse passa a plotar apenas um ponto e não linhas*/
    p.coordX = (float)(2.*X)/1280 - 1;
    p.coordY = (float)-(2.*Y)/720 + 1;
    glutPostRedisplay();
    localizaP();
  }
}

void keyboard_cb(unsigned char key, int X, int Y){
  switch(key){
      case 27:         /*27 corresponde ao ESC, e está sendo utilizado para sair*/
          exit (0);
          free(pontos); /*Libera o espaço alocado para os pontos*/
          break;
  }
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
  string trab = "  ----------------------------------------------------  TAREFA 2  ----------------------------------------------------";
  string info = "Nome: Eduardo da Silva Barbosa - DRE: 116150432";
  string comandos =  "Comandos: Botao Esquerdo do mouse desenha";
  string linha = "______________________________________________________________________________________________________________________________________";

  int len1 = (int)trab.length();
  int len2 = (int)info.length();
  int len3 = (int)comandos.length();
  int len4 = (int)linha.length();
  int len5 = (int)res.length();

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

  glRasterPos2f(0.5,0.68);
  for (int i = 0; i < len5; i++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,res[i]);
  }
}

float distancia_entre_pontos(ponto p1, ponto p2){
  return sqrt(pow(p1.coordX - p2.coordX,2)+pow(p1.coordY-p2.coordY,2)); /*Calcula a distância de ponto*/
}

void localizaP(){
  int temp = 0;/*Temp guarda a quantidade de interseção*/
  vetor v1, v2; /*V1 é o vetor "pq" e V2 assume cada um dos vetores do polígono*/
  /*Construção do vetor v1*/
  v1.p1 = p;
  v1.p2.coordX = 1.0;
  v1.p2.coordY = 0.0;
  /*Construção de cada vetor do plígono em v2 e veríficado se existe interseção com v1*/
  for(int i = 0; i<cont-1; i++){
    v2.p1 = pontos[i];
    v2.p2 = pontos[i+1];

    if(intersecao(v1,v2) == 1){
      temp++;/*Caso exista interseção é somado 1 ao temp*/
    }
  }

  if(temp%2 == 0){/*Caso temp seja par, o ponto está do lado de fora*/
    printf("FORA\n");
    res = "Resultado: FORA";
  }else{
    printf("DENTRO\n");/*Caso contrário está do lado de dentro*/
    res = "Resultado: DENTRO";
  }
}

bool intersecao(vetor v1, vetor v2){
  ponto cd, ab, ca,cb,ac,ad; /*Achando cada um dos vetores*/
  cd.coordX =  (v1.p2.coordX)-(v1.p1.coordX);
  cd.coordY =  (v1.p2.coordY)-(v1.p1.coordY);
  ca.coordX =  (v2.p2.coordX)-(v1.p1.coordX);
  ca.coordY =  (v2.p2.coordY)-(v1.p1.coordY);
  cb.coordX =  (v2.p1.coordX)-(v1.p1.coordX);
  cb.coordY =  (v2.p1.coordY)-(v1.p1.coordY);
  ab.coordX =  (v2.p2.coordX)-(v2.p1.coordX);
  ab.coordY =  (v2.p2.coordY)-(v2.p1.coordY);
  ac.coordX =  (v1.p1.coordX)-(v2.p1.coordX);
  ac.coordY =  (v1.p1.coordY)-(v2.p1.coordY);
  ad.coordX =  (v1.p2.coordX)-(v2.p2.coordX);
  ad.coordY =  (v1.p2.coordY)-(v2.p2.coordY);

 if((produto_vetorial(cd,ca)*produto_vetorial(cd,cb)< 0.0) && (produto_vetorial(ab, ac)*produto_vetorial(ab,ad) < 0.0)){
    return 1;
  }else{
    return 0;
  }
}

float produto_vetorial(ponto p1, ponto p2){ /*Produto vetorial com dois vetores com a mesma origem*/
  return (p1.coordX*p2.coordY) - (p1.coordY*p2.coordX); /*Componente Z do produto vetorial*/
}
