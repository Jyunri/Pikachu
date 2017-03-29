#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

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
GLfloat cx[3], cy[3];   //centro do rato
GLfloat ox[3], oy[3];   //origem do rato
GLfloat rx[3], ry[3];
GLfloat novo_qx, novo_qy;
GLfloat ultimo_qx, ultimo_qy;
GLfloat dx, dy;
GLfloat w = 500, h = 500, r = .0, g = .0, b = .0;
GLint count = -1;
GLfloat fator_de_escala = 1.2;

float toRadian(float grau)
{
    return grau*3.14/180;
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

    //desenha Rato
    glColor3f(r,g,b);
    glLineWidth(5.0);
    glBegin(GL_TRIANGLES);
    for(int i = 0; i < N; i++)
    {
        glVertex2f(rx[i], ry[i]);
    }
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
        for(int i = 0; i < N; i ++)
        {
            rx[i]+= dx;
            ry[i]+= dy;
            cx[i]+= dx;
            cy[i]+= dy;
            glutPostRedisplay();

        }

    }
}

void rodaRato()
{
    for(int i = 0; i < N; i ++)
    {
        float x = rx[i], y = ry[i];
        //printf("p%d: (%f,%f), c%d:(%f,%f)\n",i,rx[i],ry[i],i,cx[i],cy[i]);
        rx[i] =cx[i] + ((x-cx[i]) * 0.7 - (y-cy[i]) * 0.7); //+ (rx[i]-ox[i]);
        ry[i] =cy[i] + ((x-cx[i]) * 0.7 + (y-cy[i]) * 0.7); //+ (ry[i]-oy[i]);
        //printf("pnovo%d: (%f,%f)\n\n",i,rx[i],ry[i]);
    }


    //translada o rato para o ponto original



}

void escalaRato (unsigned char key, int x, int y)
{

    switch(key)
    {
    case '+':
        for(int i = 0; i<N; i++)
        {
            rx[i] = (rx[i]-cx[i])* fator_de_escala + cx[i];
            ry[i] = (ry[i]-cy[i])* fator_de_escala + cy[i];
        }
        break;
    case '-':
        for(int i = 0; i<N; i++)
        {
            rx[i] = (rx[i]-cx[i])*1/fator_de_escala + cx[i];
            ry[i] = (ry[i]-cy[i])*1/fator_de_escala + cy[i];
        }
        break;

    //teste para o rato correr atras do queijo apenas quando ele teclar
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

            rodaRato();

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
    cx[0] = 0, cy[0] = 0;
    cx[1] = 0, cy[1] = 0;
    cx[2] = 0, cy[2] = 0;


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
