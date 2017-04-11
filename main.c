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
GLfloat nx, ny;   //nariz do rato
GLfloat ox[3], oy[3];   //origem do rato
GLfloat rx[3], ry[3];
GLfloat novo_qx, novo_qy;
GLfloat ultimo_qx, ultimo_qy;
GLfloat dx, dy;
GLfloat w = 500, h = 500, r = .0, g = .0, b = .0;
GLint count = -1;
GLfloat scale = 1;
GLfloat theta = 0;

GLfloat corpo_x = 0.12, corpo_y = 0.17;
float tmp_x, tmp_y;

float toRadian(float grau)
{
    return grau*PI/180;
}

float toDegree(float radian)
{
    return 180*radian/PI;
}

void desenha()
{
    //limpa a area para desenhar
    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    //desenhaPikachu();
    desenhaPikachu3D();
    desenhaQueijo();


    //forca inicializacao dos comandos
    glFlush();
}

void desenhaPikachu3D()
{
    //desenha corpo
    glColor3f(255,255,0);
    //glScalef(0.5, 1.0, 1.0);
    glutWireSphere(0.5f,1,1);


}


void desenhaPikachu()
{
    //desenha corpo
    float x_original, y_original,angulo, x_transformado, y_transformado;
    glColor3f(255,255,0);
    glLineWidth(5.0);
    glBegin(GL_POLYGON);
    for (int i=0; i<=50; i++)
    {
        angulo= 2 * PI * i / 50.0;
        x_original = corpo_x*cos(angulo);
        y_original = corpo_y*sin(angulo);
        x_transformado = cos(theta)*scale*(x_original)-sin(theta)*scale*(y_original);
        y_transformado = sin(theta)*scale*(x_original)+cos(theta)*scale*(y_original);
        glVertex3f(cx+scale*x_transformado,cy+scale*y_transformado,0);
    }
    glEnd();

    //desenha nariz
    glColor3f(0,0,0);
    glPointSize(scale*3);
    glBegin(GL_POINTS);
        x_original = .002;
        y_original = .14;
        x_transformado = cos(theta)*scale*(x_original)-sin(theta)*scale*(y_original);
        y_transformado = sin(theta)*scale*(x_original)+cos(theta)*scale*(y_original);

        nx = cx+scale*x_transformado;
        ny = cy+scale*y_transformado;

        glVertex3f(cx+scale*x_transformado,cy+scale*y_transformado,0);
    glEnd();

    //desenha reta do rato
    glColor3f(0,0,1);
    glBegin(GL_LINES);
        glVertex2f(nx,ny);
        glVertex2f(cx,cy);
    glEnd();

    //desenha reta do queijo
    glColor3f(1,0,0);
    glBegin(GL_LINES);
        glVertex2f(novo_qx,novo_qy);
        glVertex2f(cx,cy);
    glEnd();

    //desenha olho esquerdo
    glColor3f(0,0,0);
    glBegin(GL_POLYGON);
    for (int i=0; i<=50; i++)
    {
        angulo= 2 * PI * i / 50.0;
        x_original = 0.015*cos(angulo)+0.04;
        y_original = 0.015*sin(angulo)+0.1;
        x_transformado = cos(theta)*scale*(x_original)-sin(theta)*scale*(y_original);
        y_transformado = sin(theta)*scale*(x_original)+cos(theta)*scale*(y_original);
        glVertex3f(cx+scale*x_transformado,cy+scale*y_transformado,0);
    }
    glEnd();

    //desenha olho direito
    glColor3f(0,0,0);
    glBegin(GL_POLYGON);
    for (int i=0; i<=50; i++)
    {
        angulo= 2 * PI * i / 50.0;
        x_original=0.015*cos(angulo)-0.04;
        y_original=0.015*sin(angulo)+0.1;
        x_transformado = cos(theta)*scale*(x_original)-sin(theta)*scale*(y_original);
        y_transformado = sin(theta)*scale*(x_original)+cos(theta)*scale*(y_original);
        glVertex3f(cx+scale*x_transformado,cy+scale*y_transformado,0);
    }
    glEnd();


    //desenha bochecha esquerda
    glColor3f(1,0,0);
    glBegin(GL_POLYGON);
    for (int i=0; i<=50; i++)
    {
        angulo= 2 * PI * i / 50.0;
        x_original=0.015*cos(angulo)+0.07;
        y_original=0.015*sin(angulo)+0.12;
        x_transformado = cos(theta)*scale*(x_original)-sin(theta)*scale*(y_original);
        y_transformado = sin(theta)*scale*(x_original)+cos(theta)*scale*(y_original);
        glVertex3f(cx+scale*x_transformado,cy+scale*y_transformado,0);
    }
    glEnd();

    //desenha bochecha direita
    glColor3f(1,0,0);
    glBegin(GL_POLYGON);
    for (int i=0; i<=50; i++)
    {
        angulo= 2 * PI * i / 50.0;
        x_original=0.015*cos(angulo)-0.07;
        y_original=0.015*sin(angulo)+0.12;
        x_transformado = cos(theta)*scale*(x_original)-sin(theta)*scale*(y_original);
        y_transformado = sin(theta)*scale*(x_original)+cos(theta)*scale*(y_original);
        glVertex3f(cx+scale*x_transformado,cy+scale*y_transformado,0);
    }
    glEnd();

    /*PARTES NAO CIRCULARES*/

    //arrays para armazenar pontos
    float tmp_x[8], tmp_y[8];
    float tmp_transformado_x[8], tmp_transformado_y[8];

    //numero de pontos
    float N;


    //desenha orelha direita
    tmp_x[0] = -.15, tmp_y[0] = -.1;
    tmp_x[1] = -.1, tmp_y[1] = .05;
    tmp_x[2] = -.11, tmp_y[2] = -.05;

    N = 3;

    for(int i = 0; i<N; i++)
    {
        tmp_transformado_x[i] = cos(theta)*scale*(tmp_x[i])-sin(theta)*scale*(tmp_y[i]);
        tmp_transformado_y[i] = sin(theta)*scale*(tmp_x[i])+cos(theta)*scale*(tmp_y[i]);
    }

    glColor3f(0,0,0);
    glPointSize(3);
    glBegin(GL_TRIANGLES);
//        glVertex2f(cos(theta)*scale*(-.15)+cx,scale*(-.1)+cy);
//        glVertex2f(cos(theta)*scale*(-.1)+cx,scale*(.05)+cy);
//        glVertex2f(cos(theta)*scale*(-.11)+cx,scale*(-.05)+cy);

    for(int i = 0; i <N; i++)
    {
        glVertex3f(cx+scale*tmp_transformado_x[i],cy+scale*tmp_transformado_y[i],0);
    }
    glEnd();

    //desenha orelha esquerda
    tmp_x[0] = .15, tmp_y[0] = -.1;
    tmp_x[1] = .1, tmp_y[1] = .05;
    tmp_x[2] = .11, tmp_y[2] = -.05;

    N = 3;

    for(int i = 0; i<N; i++)
    {
        tmp_transformado_x[i] = cos(theta)*scale*(tmp_x[i])-sin(theta)*scale*(tmp_y[i]);
        tmp_transformado_y[i] = sin(theta)*scale*(tmp_x[i])+cos(theta)*scale*(tmp_y[i]);
    }
    glColor3f(0,0,0);
    glPointSize(3);
    glBegin(GL_TRIANGLES);
    for(int i = 0; i <N; i++)
    {
        glVertex3f(cx+scale*tmp_transformado_x[i],cy+scale*tmp_transformado_y[i],0);
    }
    glEnd();

    //desenha listra 1
    tmp_x[0] = .07, tmp_y[0] = .0;
    tmp_x[1] = -.07, tmp_y[1] = .0;
    tmp_x[2] = .0, tmp_y[2] = -.02;

    N = 3;

    for(int i = 0; i<N; i++)
    {
        tmp_transformado_x[i] = cos(theta)*scale*(tmp_x[i])-sin(theta)*scale*(tmp_y[i]);
        tmp_transformado_y[i] = sin(theta)*scale*(tmp_x[i])+cos(theta)*scale*(tmp_y[i]);
    }

    glColor3f(.8,.6,0);
    glPointSize(3);
    glBegin(GL_POLYGON);
    for(int i = 0; i <N; i++)
    {
        glVertex3f(cx+scale*tmp_transformado_x[i],cy+scale*tmp_transformado_y[i],0);
    }
    glEnd();

    //desenha listra 2
    tmp_x[0] = .07, tmp_y[0] = -.05;
    tmp_x[1] = -.07, tmp_y[1] = -.05;
    tmp_x[2] = .0, tmp_y[2] = -.07;

    N = 3;

    for(int i = 0; i<N; i++)
    {
        tmp_transformado_x[i] = cos(theta)*scale*(tmp_x[i])-sin(theta)*scale*(tmp_y[i]);
        tmp_transformado_y[i] = sin(theta)*scale*(tmp_x[i])+cos(theta)*scale*(tmp_y[i]);
    }

    glColor3f(.8,.6,0);
    glPointSize(3);
    glBegin(GL_POLYGON);
    for(int i = 0; i <N; i++)
    {
        glVertex3f(cx+scale*tmp_transformado_x[i],cy+scale*tmp_transformado_y[i],0);
    }
    glEnd();

    //desenha rabo
    tmp_x[0] = -.04, tmp_y[0] = -.16;
    tmp_x[1] = -.07, tmp_y[1] = -.19;
    tmp_x[2] = -.025, tmp_y[2] = -.22;
    tmp_x[3] = -.07, tmp_y[3] = -.26;

    tmp_x[4] = .0, tmp_y[4] = -.26;
    tmp_x[5] = .025, tmp_y[5] = -.22;
    tmp_x[6] = .0, tmp_y[6] = -.19;
    tmp_x[7] = .04, tmp_y[7] = -.17;

    N = 8;

    for(int i = 0; i<N; i++)
    {
        tmp_transformado_x[i] = cos(theta)*scale*(tmp_x[i])-sin(theta)*scale*(tmp_y[i]);
        tmp_transformado_y[i] = sin(theta)*scale*(tmp_x[i])+cos(theta)*scale*(tmp_y[i]);
    }

    glColor3f(255,255,0);
    glPointSize(3);
    glBegin(GL_POLYGON);
    for(int i = 0; i <N; i++)
    {
        glVertex3f(cx+scale*tmp_transformado_x[i],cy+scale*tmp_transformado_y[i],0);
    }
    glEnd();
}

void desenhaQueijo()
{
    glColor3f(255,255,0);
    glPointSize(8);
    glBegin(GL_POINTS);
    int i;
    No *tmp = f->inicio;
    while(tmp!=NULL)
    {
        //printf("Desenhando queijo na posicao (%.3f,%.3f)\n",tmp->c.qx,tmp->c.qy);
        glVertex2d(tmp->c.qx,tmp->c.qy);
        tmp = tmp->prox;
    }
    glEnd();
}

void moveRato()
{
    printf("procurando queijo\n");
    exibeFila(f);
    printf("\n");
    while(getFirst(f))
    {
        printf("Movendo (%d,%d)\n",dx,dy);
        for(int i = 0; i < 20; i++)
        {
            cx+= dx/20;
            cy+= dy/20;
            nx+= dx/20;
            ny+= dy/20;
            desenha();
            usleep(1000*50);
            glFlush();
        }
        removeNo(f);
    }
}

float distance(float x1, float x2, float y1, float y2){
    return sqrt(pow((x1-x2),2)+pow((y1-y2),2));
}

void rodaRato()
{
    //todo definir o angulo certo
    GLfloat mr = (ny-cy)/(nx-cx);    //coeficiente angular do rato
    GLfloat mq = (f->inicio->c.qy-cy)/(f->inicio->c.qx-cx); //coeficiente angular do queijo
    float alfa = atan(((mq-mr)/(1+(mq*mr))));
    printf("m:%f n:%f, angulo: %f\n",mr,mq,toDegree(alfa));

    float tmp_angle = 0;

    //garante que ele nao ande de costaas
    float nx_rotacionado, ny_rotacionado, distancia_queijo_nariz;
    nx_rotacionado = nx*cos(alfa) - ny*sin(alfa);
    ny_rotacionado = nx*sin(alfa) + ny*cos(alfa);
    distancia_queijo_nariz = distance(nx_rotacionado,f->inicio->c.qx,ny_rotacionado,f->inicio->c.qy);

    float nx_rotacionado_suplementar, ny_rotacionado_suplementar, distancia_suplementar_queijo_nariz;
    float alfa_suplementar;
    //alfa_suplementar = alfa>0?(PI)-alfa:alfa + PI;
    alfa_suplementar = PI-alfa;

    nx_rotacionado_suplementar = nx*cos(alfa_suplementar) - ny*sin(alfa_suplementar);
    ny_rotacionado_suplementar = nx*sin(alfa_suplementar) + ny*cos(alfa_suplementar);
    distancia_suplementar_queijo_nariz = distance(nx_rotacionado_suplementar,f->inicio->c.qx,ny_rotacionado_suplementar,f->inicio->c.qy);

    printf("nx_rotacionado: %f, nx_rotacionado_suplementar: %f\n",nx_rotacionado,nx_rotacionado_suplementar);
    printf("distancia: %f\n",distancia_queijo_nariz);
    printf("distancia suplementar: %f\n",distancia_suplementar_queijo_nariz);

    if(distancia_queijo_nariz>distancia_suplementar_queijo_nariz){
       //alfa = alfa_suplementar;
       printf("suplementar: %f\n",toDegree(alfa));
    }

    while(tmp_angle < alfa)
    {
        printf("theta: %f\n",toDegree(theta));
        theta+=toRadian(10);
        tmp_angle+=toRadian(10);
        desenha();
        usleep(1000*50);
        glFlush();
    }
    if(alfa < 0)
    {
        while(tmp_angle > alfa)
        {
            printf("theta: %f\n",toDegree(theta));
            theta-=toRadian(10);
            tmp_angle-=toRadian(10);
            desenha();
            usleep(1000*50);
            glFlush();
        }

    }

}

void escalaRato (unsigned char key, int x, int y)
{

    switch(key)
    {
    case '+':
        if(scale<5)
            scale+=.2;
        break;
    case '-':
        if(scale>.2)
            scale-=.2;
        break;

    //teste: move para origem
    case 'o':
        tmp_x = cx;
        tmp_y = cy;
        cx = 0;
        cy = 0;
        corpo_x = (corpo_x-cx)*scale;
        corpo_y = (corpo_y-cy)*scale;
        cx = tmp_x;
        cy = tmp_y;

    //teste: roda rato
    case 'r':
        theta+=toRadian(90);

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

            if(novo_qx>.7)  novo_qx = .7;
            else if(novo_qx<-.7) novo_qx = -.7;

            if(novo_qy>.7)  novo_qy = .7;
            else if(novo_qy<-.7) novo_qy = -.7;

            //Insere a posicao do queijo na Fila
            printf("Tentando inserir queijo na posicao (%.3f,%.3f)\n",novo_qx,novo_qy);
            insereNo(f);
            exibeFila(f);

            rodaRato();

            //TODO - tem que fazer o rato mover aos poucos
            moveRato(); //leva o rato ate o queijo criado
        }
        break;
    case GLUT_RIGHT_BUTTON:
        moveRato();
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

//recebe primeiro elemento da fila mas nao desenfileira
int getFirst(Fila *f)
{
    if(vazia(f))
    {
        printf("Fila vazia! Acabou queijos!\n");
        return 0;
    }
    No *tmp = f->inicio;

    //recebe distancia entre primeiro queijo da fila e o ultimo queijo
    dx = tmp->c.qx - ultimo_qx;
    dy = tmp->c.qy - ultimo_qy;

    return 1;

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
        //dx = tmp->c.qx - ultimo_qx;
        //dy = tmp->c.qy - ultimo_qy;

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

    //nariz do rato
    nx = .002, ny = .14;

    f = (Fila*)malloc(sizeof(Fila));
    iniciaFila(f);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(50,100);
    glutInitWindowSize(w,h);
    glutCreateWindow("Pikachu");
    glutDisplayFunc(desenha);
    glutMouseFunc(posicionaQueijo);
    glutKeyboardFunc(escalaRato);
    //glutSpecialFunc(SpecialKeys);
    glutMainLoop ();

    free(f);
    return 0;
}
