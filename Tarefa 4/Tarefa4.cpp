/*
Nome: Eduardo da Silva Barbosa
DRE: 116150432
---------TAREFA 4--------------
Objetivo: fazer a triangulação de delaunay a partir de um fecho convexo.
*/

/*
compilação: g++ -o Tarefa4 Tarefa4.cpp -lglut -lGLU -lGL -Wall -ansi
*/

/*Bibliotecas Utilizadas*/
#include<stdlib.h>
#include<stdio.h>
#include<GL/gl.h>
#include<GL/freeglut.h>
#include<math.h>
#include <iostream>
#include <time.h>

using namespace std;

typedef struct _Coordenadas{ /*Estrutura que define um ponto ou um vetor, com as coordenadas 'x' e 'y'.*/
    float coordX;
    float coordY;
} Coordenadas;

typedef struct _Triangulo{ /*Estrutura que define um triângulo, com os pontos P1, P2 e P3.*/
    Coordenadas P1;
    Coordenadas P2;
    Coordenadas P3;
    struct _Triangulo *next = NULL; /*next pode ser utilizado para a criação de uma lista encadeada de triângulos.*/
} Triangulo;

typedef struct _Aresta{ /*Estrutura que define uma aresta, com os pontos da extremidades (P1 e P2).*/
    Coordenadas P1;
    Coordenadas P2;
    struct _Aresta *next = NULL; /*next pode ser utilizado para a criação de uma lista encadeada de arestas.*/
} Aresta;

/*Protótipos das funções utilizadas*/
void display(); /*Função responsável por plotar na tela*/
void mouse(int button, int state, int X, int Y);
void keyboard_cb(unsigned char key, int X, int Y);
Coordenadas *resize(Coordenadas *p, int Tamanho, int NovoTam); /*Função responsável por redimensionar o tamanho na memória para os pontos*/
void cabecalho(); /*Função responsável por renderizar o cabeçalho*/
void fechoConvexo(); /*Encontra o Fecho Convexo de uma nuvem de pontos*/
Coordenadas pontoExtremo(); /*Acha o ponto mais extremo (menor) em relação ao Y*/
float angulo_entre_vetores(Coordenadas vetor1, Coordenadas vetor2); /*Encontra o angulo entre dois vetores com a mesma origem*/
Coordenadas acha_vetor(Coordenadas origem, Coordenadas destino); /*Acha o vetor com a origem em (0,0)*/
bool contem_ponto(Coordenadas *array, int tam, Coordenadas pontos); /*Verifica se um ponto específico está contido em um array de pontos*/
float produto_interno(Coordenadas vetor1, Coordenadas vetor2); /*Produto interno com dois vetores de mesma origem*/
float produto_vetorial(Coordenadas vetor1, Coordenadas vetor2); /*Produto vetorial com dois vetores de mesma origem*/
float modulo_vetor(Coordenadas vetor); /*Encontra o módulo/comprimento de um vetor*/
bool compara_pontos(Coordenadas ponto1, Coordenadas ponto2); /*Verifica se dois pontos são iguais*/
void triangulacao(); /*Gera a trinagulação de Delaunay a partir do fecho conexo */
void acha_trinagulo(Aresta A); /*Encontra um triângulo a partir de uma determinada aresta A.*/
bool colineares(Coordenadas p1, Coordenadas p2, Coordenadas p3); /*Verifico se 3 pontos consecutivos são colineares*/
float angulo_oposto_aresta(Coordenadas p1, Coordenadas p2, Coordenadas p3); /*p1 e p2 são as extremidades da aresta e p3 o vértice o qual quero achar o ângulo*/
bool contem_triangulo(Triangulo T); /*Verifica se o triângulo T está na lista encadeada de triângulos da triangulação*/
void adicionar_triangulo(Coordenadas p1, Coordenadas p2, Coordenadas p3); /*Adiciona o triângulo (p1,p2,p3) a lista encadeada de triângulos da triaangulação */
bool intersecao(Coordenadas p1, Coordenadas p2); /*Verifica se ocorre interseção entre a aresta (p1,p2) com qualquer aresta pertencente a um dos triângulos da triangulação*/
bool fecho_contemAresta(Coordenadas p1, Coordenadas p2); /*Verifica se a aresta (p1,p2) pertence ao fecho convexo*/
bool intersecao_entreDuasArestas(Coordenadas p1, Coordenadas p2, Coordenadas p3, Coordenadas p4); /*p1 e p2 são as extremidades da primeira aresta e p3 e p4 as extremidades da segunda aresta*/
bool sobrepoe_triangulos(Triangulo T); /*Verifica se dentro de T existe algum triângulo do fecho convexo*/
bool triangulo_contemPonto(Triangulo T, Coordenadas p); /*Verifica se o ponto p está dentro ou fora do triângulo T*/
void adicionar_aresta(Coordenadas p1 ,Coordenadas p2); /*Adiciona a aresta (p1, p2) a lista encadeada de arestas que devem ser exploradas*/

/*Variáveis globais*/
int cont = 0; /*Cont é a quantidade de pontos existentes*/
int contp;/*Contp é a quantidade de pontos existentes no fecho convexo*/
int tam = 100; /*Tamanho do array que vai guardar os pontos, inicialmente começa com 100 e cresce conforme a necessidade.*/
Coordenadas *pontos = (Coordenadas *) malloc (sizeof(Coordenadas)*tam);/*Alocação de memória para o array de pontos*/
Coordenadas *pontosP; /*pontos pertecentes ao fecho convexo*/
Triangulo *triangulos = NULL; /*Lista que guardará os triângulos da triangulação*/
Aresta *filaArestas = NULL; /*Lista que guardará as arestas que devem ser exploradas, com o objetivo de achar os triângulos da triangulação*/

int main(int argc, char **argv){

  if(!pontos){ /*Verifica se a memória foi alocada para 'pontos'.*/
          printf("Erro: Nao foi possível alocar memória.\n");
          exit(1);
        }

    glutInit(&argc,argv); /*Esta função é utilizada para iniciar a biblioteca GLUT.*/
    /*glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);*/
    /*glutInitWindowPosition(200,200); *//*Posição da janela na tela*/
    glutInitWindowSize (1280, 720); /*Tamanho da Janela*/
    glutCreateWindow("Tarefa 4 - Eduardo da Silva Barbosa"); /*Nome da Janela*/
    glutKeyboardFunc(keyboard_cb);/*Função que será chamada quando ocorrer um evento no teclado*/
    glutMouseFunc(mouse);/*Função que será chamada quando ocorrer um evento no mouse*/
    glutDisplayFunc(display); /*Função que será chamada quando houver a necessidade de atualizar a janela*/
    glutMainLoop(); /*Inicia o gerenciamento de eventos, aguardando que algum evento seja acionado.*/
    return EXIT_SUCCESS ;

}

void display(){
  Triangulo *temp;

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  cabecalho();
  glPointSize(8.0); /*Define o tamanho do ponto*/
  glLineWidth(4); /*Define a espessura da linha*/

  glBegin(GL_TRIANGLES);/*Plota as linhas dos triangulos*/
  srand( (unsigned)time(NULL) );
  temp = triangulos;
  while(temp != NULL){
    glColor3f( (rand()%101)/100.0, (rand()%101)/100.0, (rand()%101)/100.0); /*As cores são definidas de modo aleatório*/
    glVertex2f((*temp).P1.coordX,(*temp).P1.coordY);
    glVertex2f((*temp).P2.coordX,(*temp).P2.coordY);
    glVertex2f((*temp).P3.coordX,(*temp).P3.coordY);
    temp = (*temp).next;
  }
  glEnd();

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
      case 't':
          if(triangulos != NULL){
            free(triangulos);
            triangulos = NULL;
            free(filaArestas);
            filaArestas = NULL;
          }
          fechoConvexo();
          triangulacao();
          glutPostRedisplay();
          break;
      case 'T':
          if(triangulos != NULL){
            free(triangulos);
            triangulos = NULL;
            free(filaArestas);
            filaArestas = NULL;
          }
          fechoConvexo();
          triangulacao();
          glutPostRedisplay();
          break;
  }
}

Coordenadas *resize(Coordenadas *p, int Tamanho, int NovoTam){
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

void cabecalho(){
  string trab = "  ----------------------------------------------------  TAREFA 4  ----------------------------------------------------";
  string info = "Nome: Eduardo da Silva Barbosa - DRE: 116150432";
  string comandos =  "Comandos: Botao Esquerdo do mouse: Desenha Pontos || Tecla F: Gera o fecho convexo || Tecla T: Gera a triangulacao";
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

void fechoConvexo(){
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

Coordenadas pontoExtremo(){
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

Coordenadas acha_vetor(Coordenadas origem, Coordenadas destino){
  Coordenadas vetor;
  vetor.coordX = destino.coordX - origem.coordX;
  vetor.coordY = destino.coordY - origem.coordY;
  return vetor;
}

float angulo_entre_vetores(Coordenadas vetor1, Coordenadas vetor2){
    return acos(produto_interno(vetor1,vetor2)/(modulo_vetor(vetor1)*modulo_vetor(vetor2)));
}

bool contem_ponto(Coordenadas *array, int tam, Coordenadas ponto){
  for(int i = 0; i<tam; i++){
    if((array[i].coordX == ponto.coordX) && (array[i].coordY == ponto.coordY)){
      return 1;
    }
  }
  return 0;
}

float produto_interno(Coordenadas vetor1, Coordenadas vetor2){
  return (vetor1.coordX*vetor2.coordX) + (vetor1.coordY*vetor2.coordY);
}

float produto_vetorial(Coordenadas vetor1, Coordenadas vetor2){
  return (vetor1.coordX*vetor2.coordY) - (vetor1.coordY*vetor2.coordX); /*Componente Z do produto vetorial, uma vez que as componentes entre X e Y são zero.*/
}

float modulo_vetor(Coordenadas vetor){
  return sqrt(pow(vetor.coordX,2)+pow(vetor.coordY,2));
}

bool compara_pontos(Coordenadas ponto1, Coordenadas ponto2){
  if((ponto1.coordX == ponto2.coordX) && (ponto1.coordY == ponto2.coordY)){
    return 1;
  }else{
    return 0;
  }
}

void triangulacao(){
  Aresta *aresta, primeiraAresta;
  primeiraAresta.P1 = pontosP[0];
  primeiraAresta.P2 = pontosP[1];

  acha_trinagulo(primeiraAresta);
  aresta = filaArestas;

  while(aresta != NULL){
    acha_trinagulo(*aresta);
    aresta = (*aresta).next;
  }

}

void acha_trinagulo(Aresta A){
  int i;
  Coordenadas p3;
  Triangulo temp;
  p3.coordX = -100;
  for(i = 0; i < cont; i++){
    temp.P1 = A.P1;
    temp.P2 = A.P2;
    temp.P3 = pontos[i];
    if(!compara_pontos(pontos[i],A.P1) && !compara_pontos(pontos[i],A.P2) && !colineares(A.P1, A.P2, pontos[i]) && !contem_triangulo(temp) && !intersecao(A.P1, pontos[i]) && !intersecao(A.P2, pontos[i]) && !sobrepoe_triangulos(temp)){
      if(p3.coordX == -100){
        p3.coordX = pontos[i].coordX;
        p3.coordY = pontos[i].coordY;
      }
      if(angulo_oposto_aresta(A.P1, A.P2, pontos[i])>angulo_oposto_aresta(A.P1, A.P2, p3)){
        p3.coordX = pontos[i].coordX;
        p3.coordY = pontos[i].coordY;
      }
    }
  }

  if(p3.coordX != -100){
     adicionar_triangulo(A.P1, A.P2, p3);
     if(!fecho_contemAresta(A.P1, p3)){
       adicionar_aresta(A.P1, p3);
     }
     if(!fecho_contemAresta(A.P2, p3)){
       adicionar_aresta(A.P2, p3);
    }
  }
}

bool colineares(Coordenadas p1, Coordenadas p2, Coordenadas p3){
  float determinante, temp1, temp2;
  float matrix[3][3];

  for(int i =0; i<3; i++){
    matrix[i][2] = 1;
  }

  /*Monto a matriz com cada coordenada dos pontos*/
  matrix[0][0] = p1.coordX;
  matrix[0][1] = p1.coordY;
  matrix[1][0] = p2.coordX;
  matrix[1][1] = p2.coordY;
  matrix[2][0] = p3.coordX;
  matrix[2][1] = p3.coordY;

  /*Calculo o determinante e verifico se é igual a 0 ou não*/
  temp1 =  matrix[0][0]*matrix[1][1]*matrix[2][2] + matrix[0][1]*matrix[1][2]*matrix[2][0] + matrix[1][0]*matrix[2][1]*matrix[2][2];
  temp2 =  matrix[0][2]*matrix[1][1]*matrix[2][0] + matrix[0][0]*matrix[1][2]*matrix[2][1] + matrix[1][0]*matrix[0][1]*matrix[2][2];
  determinante = temp1 - temp2;

  return (determinante == 0) ? 1 : 0;
}

float angulo_oposto_aresta(Coordenadas p1, Coordenadas p2, Coordenadas p3){
  Coordenadas vetor1, vetor2;
  vetor1.coordX = p1.coordX-p3.coordX;
  vetor1.coordY = p1.coordY-p3.coordY;
  vetor2.coordX = p2.coordX-p3.coordX;
  vetor2.coordY = p2.coordY-p3.coordY;
  return angulo_entre_vetores(vetor1,vetor2);
}

bool contem_triangulo(Triangulo T){
  Triangulo *temp;
  temp = triangulos;
  while(temp != NULL){
    if((compara_pontos((*temp).P1,T.P1)||compara_pontos((*temp).P1,T.P2)||compara_pontos((*temp).P1,T.P3)) && (compara_pontos((*temp).P2,T.P1)||compara_pontos((*temp).P2,T.P2)||compara_pontos((*temp).P2,T.P3)) &&
    (compara_pontos((*temp).P3,T.P1)||compara_pontos((*temp).P3,T.P2)||compara_pontos((*temp).P3,T.P3))){
      return 1;
    }
    temp = (*temp).next;
  }
  return 0;
}

void adicionar_triangulo(Coordenadas p1, Coordenadas p2, Coordenadas p3){
  Triangulo *t, *temp;
  t = (Triangulo *) malloc (sizeof(Triangulo));
  temp = triangulos;
  (*t).P1.coordX = p1.coordX;
  (*t).P1.coordY = p1.coordY;
  (*t).P2.coordX = p2.coordX;
  (*t).P2.coordY = p2.coordY;
  (*t).P3.coordX = p3.coordX;
  (*t).P3.coordY = p3.coordY;
  (*t).next = NULL;
  if(temp == NULL){
    triangulos = t;
    return;
  }

  while((*temp).next != NULL){
    temp = (*temp).next;
  }
  (*temp).next = t;
}

bool intersecao(Coordenadas p1, Coordenadas p2){
  Triangulo *temp;
  temp = triangulos;
  while(temp != NULL){
    if(intersecao_entreDuasArestas(p1, p2, (*temp).P1,(*temp).P2) || intersecao_entreDuasArestas(p1, p2, (*temp).P1,(*temp).P3) || intersecao_entreDuasArestas(p1, p2, (*temp).P3,(*temp).P2)){
      return 1;
    }
    temp = (*temp).next;
  }
  return 0;

}

bool fecho_contemAresta(Coordenadas p1, Coordenadas p2){
  int i=0;
  while(i<contp-1){
    if((compara_pontos(pontosP[i], p1) && compara_pontos(pontosP[i+1], p2)) || (compara_pontos(pontosP[i], p2) && compara_pontos(pontosP[i+1], p1))){
      return 1;
    }
    i++;
  }
  return 0;
}

bool intersecao_entreDuasArestas(Coordenadas p1, Coordenadas p2, Coordenadas p3, Coordenadas p4){
  Coordenadas cd, ab, ca, cb, ac, ad; /*Achando cada um dos vetores*/
  cd = acha_vetor(p1,p2);
  ca = acha_vetor(p1,p3);
  cb = acha_vetor(p1,p4);
  ab = acha_vetor(p3,p4);
  ac = acha_vetor(p3,p1);
  ad = acha_vetor(p3,p2);

  if((produto_vetorial(cd,ca)*produto_vetorial(cd,cb)< 0.0) && (produto_vetorial(ab, ac)*produto_vetorial(ab,ad) < 0.0)){
     return 1;
  }else{
    return 0;
  }
}

bool sobrepoe_triangulos(Triangulo T){
  Triangulo *temp;
  temp = triangulos;
  while(temp != NULL){
    if(triangulo_contemPonto(T,(*temp).P1) && triangulo_contemPonto(T,(*temp).P2) && triangulo_contemPonto(T,(*temp).P3)){
      return 1;
    }
    temp = (*temp).next;
  }
  return 0;
}/**/

bool triangulo_contemPonto(Triangulo T, Coordenadas p1){
  int temp = 0;/*Temp guarda a quantidade de interseção*/
  Coordenadas p2;
  p2.coordX = 1.0;
  p2.coordY = 0.0;

    if(intersecao_entreDuasArestas(p1, p2, T.P1, T.P2)){
      temp++;/*Caso exista interseção é somado 1 ao temp*/
    }

    if(intersecao_entreDuasArestas(p1, p2, T.P1, T.P3)){
      temp++;/*Caso exista interseção é somado 1 ao temp*/
    }

    if(intersecao_entreDuasArestas(p1, p2, T.P3, T.P2)){
      temp++;/*Caso exista interseção é somado 1 ao temp*/
    }

  if(temp%2 == 0){/*Caso temp seja par, o ponto está do lado de fora*/
    return 0;
  }else{
    return 1;
  }
}

void adicionar_aresta(Coordenadas p1 ,Coordenadas p2){
  Aresta *arestas, *temp;
  arestas = filaArestas;

  temp = (Aresta *) malloc (sizeof(Aresta));
  (*temp).P1.coordX = p1.coordX;
  (*temp).P1.coordY = p1.coordY;
  (*temp).P2.coordX = p2.coordX;
  (*temp).P2.coordY = p2.coordY;
  (*temp).next = NULL;
  if(arestas == NULL){
    filaArestas = temp;
    return;
  }

  while((*arestas).next != NULL){
    arestas = (*arestas).next;
  }
  (*arestas).next = temp;
}
