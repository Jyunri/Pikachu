#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <unistd.h>

#define PI M_PI

typedef struct
{
    float qx;
    float qy;
} Coordenadas;

typedef struct
{
    Coordenadas c;
    int valor;
    struct No *prox;
} No;

typedef struct
{
    No *inicio;
    No *fim;
    int tamanho;
} Fila;

Fila *f;
GLint N = 3;  //numero de coordenadas do rato
GLfloat X1 = -.1, Y1 = 0, X2 = .1, Y2 = 0, X3 = 0 , Y3 = .1;
GLfloat cx, cy;   //centro do rato
GLfloat ox[3], oy[3];   //origem do rato
GLfloat rx[3], ry[3];
GLfloat novo_qx, novo_qy;
GLfloat ultimo_qx, ultimo_qy;
GLfloat dx, dy;
GLfloat w = 500, h = 500, r = .0, g = .0, b = .0;
GLint count = -1;
GLfloat fator_de_escala = 1.2;
GLfloat angle;

GLfloat corpo_x = 0.12, corpo_y = 0.17;
float tmp_x, tmp_y;

float toRadian(float grau)
{
    return grau*M_PI/180;
}

void SpecialKeys (int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_LEFT:
    case GLUT_KEY_UP:
        glShadeModel(GL_SMOOTH);
        break;
    case GLUT_KEY_RIGHT:
    case GLUT_KEY_DOWN:
        glShadeModel(GL_FLAT);
        break;
    }
    glutPostRedisplay();
}

void desenha()
{
    //limpa a area para desenhar
    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    //desenha Triangulo teste
//    glColor3f(255,255,0);
//    glLineWidth(5.0);
//
//    glBegin(GL_TRIANGLES);
//    for(int i = 0; i < N; i++)
//    {
//        glVertex2f(rx[i], ry[i]);
//    }
//    glEnd();

    //desenha corpo
    float angulo;
    glColor3f(255,255,0);
    glLineWidth(5.0);
    glBegin(GL_POLYGON);
    for (int i=0; i<=50; i++)
    {
        angulo= 2 * PI * i / 50.0;
        glVertex3f(cx+corpo_x*cos(angulo), cy+corpo_y*sin(angulo),0);
    }
    glEnd();

    //desenha nariz
    glColor3f(0,0,0);
    glBegin(GL_POLYGON);
    for (int i=0; i<=50; i++)
    {
        angulo= 2 * PI * i / 50.0;
        glVertex3f(cx+0.008*cos(angulo), cy +0.008*sin(angulo)+0.14,0);
    }
    glEnd();

    //desenha olho esquerdo
    glColor3f(0,0,0);
    glBegin(GL_POLYGON);
    for (int i=0; i<=50; i++)
    {
        angulo= 2 * PI * i / 50.0;
        glVertex3f(cx+0.015*cos(angulo)+0.04, cy +0.015*sin(angulo)+0.1,0);
    }
    glEnd();

    //desenha olho direito
    glColor3f(0,0,0);
    glBegin(GL_POLYGON);
    for (int i=0; i<=50; i++)
    {
        angulo= 2 * PI * i / 50.0;
        glVertex3f(cx+0.015*cos(angulo)-0.04, cy +0.015*sin(angulo)+0.1,0);
    }
    glEnd();


    //desenha bochecha esquerda
    glColor3f(1,0,0);
    glBegin(GL_POLYGON);
    for (int i=0; i<=50; i++)
    {
        angulo= 2 * PI * i / 50.0;
        glVertex3f(cx+0.015*cos(angulo)+0.07, cy +0.015*sin(angulo)+0.12,0);
    }
    glEnd();

    //desenha bochecha direita
    glColor3f(1,0,0);
    glBegin(GL_POLYGON);
    for (int i=0; i<=50; i++)
    {
        angulo= 2 * PI * i / 50.0;
        glVertex3f(cx+0.015*cos(angulo)-0.07, cy +0.015*sin(angulo)+0.12,0);
    }
    glEnd();

    //desenha orelha direita
    glColor3f(0,0,0);
    glPointSize(3);
    glBegin(GL_TRIANGLES);
        glVertex2f(-.15+cx,-.1+cy);
        glVertex2f(-.1+cx,.05+cy);
        glVertex2f(-.11+cx,-.05+cy);
    glEnd();

    //desenha orelha esquerda
    glColor3f(0,0,0);
    glPointSize(3);
    glBegin(GL_TRIANGLES);
        glVertex2f(.15+cx,-.1+cy);
        glVertex2f(.1+cx,.05+cy);
        glVertex2f(.11+cx,-.05+cy);
    glEnd();

    //desenha listra 1
    glColor3f(.8,.6,0);
    glPointSize(3);
    glBegin(GL_POLYGON);
        glVertex2f(.07+cx,.0+cy);
        glVertex2f(-.07+cx,0+cy);
        glVertex2f(0+cx,-.02+cy);
    glEnd();

    //desenha listra 2
    glColor3f(.8,.6,0);
    glPointSize(3);
    glBegin(GL_POLYGON);
        glVertex2f(.07+cx,-.05+cy);
        glVertex2f(-.07+cx,-.05+cy);
        glVertex2f(0+cx,-.07+cy);
    glEnd();

    //desenha rabo
    glColor3f(255,255,0);
    glPointSize(3);
    glBegin(GL_POLYGON);
        glVertex2f(-.04+cx,-.16+cy);
        glVertex2f(-.07+cx,-.19+cy);
        glVertex2f(-0.025+cx,-.22+cy);
        glVertex2f(-.07+cx,-.26+cy);

        glVertex2f(.0+cx,-.26+cy);
        glVertex2f(0.025+cx,-.22+cy);
        glVertex2f(.0+cx,-.19+cy);
        glVertex2f(.04+cx,-.16+cy);
    glEnd();

    //desenha Queijo
    glColor3f(255,255,0);
    glPointSize(8);
    glBegin(GL_POINTS);
    int i;

    No *tmp = f->inicio;
    while(tmp!=NULL)
    {
        printf("Desenhando queijo na posicao (%.3f,%.3f)\n",tmp->c.qx,tmp->c.qy);
        glVertex2d(tmp->c.qx,tmp->c.qy);
        tmp = tmp->prox;
    }
//    for(i = 0; i <= count; i++)
//    {
//        glVertex2f(novo_qx,novo_qy);
//    }
    glEnd();

    //forca inicializacao dos comandos
    glFlush();
}

void moveRato()
{
    printf("procurando queijo\n");
    exibeFila(f);
    printf("\n");
    if(removeNo(f))
    {
        printf("Movendo (%d,%d)\n",dx,dy);
        //rodaRato(); //roda de acordo com a posicao do queijo
        cx+= dx;
        cy+= dy;


        for(int i = 0; i < N; i ++)
        {
            rx[i]+= dx;
            ry[i]+= dy;
            glutPostRedisplay();
        }

    }
}

void rodaRato()
{
    //todo definir o angulo certo
    GLfloat m = (ry[3]-cy)/(rx[3]-cx);    //coeficiente angular do rato
    GLfloat n = (novo_qy-cy)/(novo_qx-cx);
    angle = atan(abs((m-n)/(1+(m*n))));
    printf("m:%f n:%f, angulo: %f\n",m,n,angle);

    angle = toRadian(90);

    /*rotaciona triangulo
    for(int i = 0; i < N; i ++)
    {
        float x = rx[i], y = ry[i];
        //printf("p%d: (%f,%f), c%d:(%f,%f)\n",i,rx[i],ry[i],i,cx[i],cy[i]);
        rx[i] =cx + ((x-cx) * cos(angle) - (y-cy) * sin(angle)); //+ (rx[i]-ox[i]);
        ry[i] =cy + ((x-cx) * sin(angle) + (y-cy) * cos(angle)); //+ (ry[i]-oy[i]);
        //printf("pnovo%d: (%f,%f)\n\n",i,rx[i],ry[i]);
    }
    */

    float x = corpo_x, y = corpo_y;
    //printf("p%d: (%f,%f), c%d:(%f,%f)\n",i,rx[i],ry[i],i,cx[i],cy[i]);
    corpo_x = cx + ((x-cx) * cos(angle) - (y-cy) * sin(angle)); //+ (rx[i]-ox[i]);
    corpo_y = cy + ((x-cx) * sin(angle) + (y-cy) * cos(angle)); //+ (ry[i]-oy[i]);


    //translada o rato para o ponto original



}

void escalaRato (unsigned char key, int x, int y)
{

    switch(key)
    {
    case '+':
        for(int i = 0; i<N; i++)
        {
            rx[i] = (rx[i]-cx)* fator_de_escala + cx;
            ry[i] = (ry[i]-cy)* fator_de_escala + cy;
        }
        corpo_x = (corpo_x-cx)*fator_de_escala + cx;
        corpo_y = (corpo_y-cy)*fator_de_escala + cy;
        break;
    case '-':
        for(int i = 0; i<N; i++)
        {
            rx[i] = (rx[i]-cx)*1/fator_de_escala + cx;
            ry[i] = (ry[i]-cy)*1/fator_de_escala + cy;
        }
        corpo_x = (corpo_x-cx)*1/fator_de_escala + cx;
        corpo_y = (corpo_y-cy)*1/fator_de_escala + cy;
        break;

    //teste: move para origem
    case 'o':
        tmp_x = cx;
        tmp_y = cy;
        cx = 0;
        cy = 0;
        corpo_x = (corpo_x-cx)*fator_de_escala;
        corpo_y = (corpo_y-cy)*fator_de_escala;
        cx = tmp_x;
        cy = tmp_y;


    //teste: faz o rato correr atras do queijo apenas quando ele teclar
    case 'm':
        moveRato();
        break;
    }
    glutPostRedisplay();
}


void posicionaQueijo (int botao, int estado, int x, int y)
{
    switch(botao)
    {
    case GLUT_LEFT_BUTTON:
        if(estado == GLUT_DOWN)
        {
            count++;
            novo_qx = ((float)x/((float)w/2.0))-1.0;
            novo_qy = (float) 1 - y/(h/2.0);

            //Insere a posicao do queijo na Fila
            printf("Tentando inserir queijo na posicao (%.3f,%.3f)\n",novo_qx,novo_qy);
            insereNo(f);
            exibeFila(f);

            //rodaRato();

            //TODO - tem que fazer o rato mover aos poucos
            //moveRato(); //leva o rato ate o queijo criado
        }
        break;
    default:
        break;
    }
    glutPostRedisplay();
}


void iniciaFila(Fila *f)
{
    f->inicio = NULL;
    f->fim = NULL;
    f->tamanho = 0;
}

int vazia(Fila *f)
{
    return (f->inicio == NULL);
}

void exibeFila(Fila *f)
{
    if(vazia(f))
    {
        printf("fila vazia");
    }
    else
    {
        No *tmp  = f->inicio;
        printf("Fila: ");
        while(tmp->prox!=NULL)
        {
            printf("(%.3f,%.3f), ",tmp->c.qx,tmp->c.qy);
            tmp = tmp->prox;
        }
        printf("(%.3f,%.3f)\n",tmp->c.qx,tmp->c.qy);   //ultimo elemento
    }

}

void insereNo(Fila *f)
{
    printf("Recebido queijo na posicao (%.3f,%.3f)\n",novo_qx,novo_qy);
    No *no=(No*)malloc(sizeof(No));
    no->c.qx=novo_qx;
    no->c.qy=novo_qy;
    no->prox = NULL;
    if(vazia(f))
    {
        f->inicio = no;
        f->fim = no;
    }
    else
    {
        f->fim->prox = no;
        f->fim = no;
    }
    printf("Inserido queijo na posicao (%.3f,%.3f)\n",novo_qx,novo_qy);
    f->tamanho++;
}

int removeNo(Fila *f)
{
    if(vazia(f))
    {
        printf("Fila vazia! Acabou queijos!\n");
        return 0;
    }
    else
    {
        No *tmp = f->inicio;

        //recebe distancia entre primeiro queijo da fila e o ultimo queijo
        dx = tmp->c.qx - ultimo_qx;
        dy = tmp->c.qy - ultimo_qy;

        //ultimo queijo passa a ser o primeiro da fila
        ultimo_qx = tmp->c.qx;
        ultimo_qy = tmp->c.qy;

        //remove primeiro da fila
        free(f->inicio);
        f->inicio = tmp->prox;
        f->tamanho--;

        if (f->inicio == NULL)
            f->fim = NULL;


        return 1;
    }

}

int main(int argc, char *argv[ ] )
{
    //Posicao inicial do queijo (nao aparece)
    ultimo_qx = 0;
    ultimo_qy = 0;

    //centro do rato
    cx = 0, cy = 0;


    //origem para o rato
    ox[0] = -.1, oy[0] = 0;
    ox[1] = .1, oy[1] = 0;
    ox[2] = 0 , oy[2] = .1;

    //posicao inicial do rato
    rx[0] = -.1, ry[0] = 0;
    rx[1] = .1, ry[1] = 0;
    rx[2] = 0 , ry[2] = .1;

    f = (Fila*)malloc(sizeof(Fila));
    iniciaFila(f);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(50,100);
    glutInitWindowSize(w,h);
    glutCreateWindow("Exercicio 1");
    glutDisplayFunc(desenha);
    glutMouseFunc(posicionaQueijo);
    glutKeyboardFunc(escalaRato);
    glutSpecialFunc(SpecialKeys);
    glutMainLoop ();

    free(f);
    return 0;
}
