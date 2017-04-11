#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define PI M_PI

typedef struct
{
    float qx;
    float qy;
    float qz;
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
GLint angx = 0, angy = 0, angz = 0;
GLfloat scale = .4;
GLint w = 700, h = 500;
GLfloat cx = 0, cy = 0, cz = 0; //centro do pikachu
GLfloat nx = -.75, ny = 0, nz = 0; //nariz do pikachu
GLfloat ultimo_qx = 0, ultimo_qy = 0, ultimo_qz = 0;
GLfloat dx = 0, dy = 0, dz = 0;
GLfloat translada_x =0, translada_y = 0, translada_z = 0;
GLfloat novo_qx = 0, novo_qy = 0, novo_qz = 0;

//variaveis de movimento das patas
GLint angulo_coxa_fe = 50;
GLint angulo_perna_fe = -50;
int par1 = 1;   //frontal esquerda, traseira direita
float translada_par1 = .1;

//funcoes angulo e distancia
float toRadian(float grau)
{
    return grau*PI/180;
}

float toDegree(float radian)
{
    return 180*radian/PI;
}

float distance(float x1, float x2, float y1, float y2){
    return sqrt(pow((x1-x2),2)+pow((y1-y2),2));
}


//funcoes FILA
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
            printf("(%.3f,%.3f,%.3f), ",tmp->c.qx,tmp->c.qy,tmp->c.qz);
            tmp = tmp->prox;
        }
        printf("(%.3f,%.3f,%.3f)\n",tmp->c.qx,tmp->c.qy,tmp->c.qz);   //ultimo elemento
    }

}

void insereNo(Fila *f)
{
    printf("Recebido queijo na posicao (%.3f,%.3f,%.3f)\n",novo_qx,novo_qy,novo_qz);
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
    printf("Inserido queijo na posicao (%.3f,%.3f,%.3f)\n",novo_qx,novo_qy,novo_qz);
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

    //animacao
        //recebe distancia entre primeiro queijo da fila e o ultimo queijo
    dx = tmp->c.qx - ultimo_qx;
    dy = tmp->c.qy - ultimo_qy;
    dz = tmp->c.qz - ultimo_qz;

    printf("Distancia entre ultimo queijo e novo queijo: (%.3f,%.3f,%.3f)\n",dx,dy,dz);

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

        //animacao
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



//funcoes OPENGL
void display(void)
{
    glClearColor (1, 1, 1, 1);
    glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    //eixo x
    glColor3f(1,0,0);
    glLineWidth(3);
    glBegin(GL_LINES);
        glVertex3f(1,0,0);
        glVertex3f(-1,0,0);
    glEnd();

    //eixo y
    glColor3f(0,1,0);
    glBegin(GL_LINES);
        glVertex3f(0,1,0);
        glVertex3f(0,-1,0);
    glEnd();

    //eixo z
    glColor3f(0,0,1);
    glBegin(GL_LINES);
        glVertex3f(0,0,1);
        glVertex3f(0,0,-1);
    glEnd();

    //desenha reta do queijo
    glColor3f(1,0,0);
    glBegin(GL_LINES);
        glVertex3f(novo_qx,novo_qy,novo_qz);
        glVertex3f(cx,cy,cz);
    glEnd();

    glPushMatrix();
        //operacoes em todo o pikachu
        glTranslatef(translada_x,translada_y,translada_z);
        glRotatef(angx,1.0,0.0,0.0);
        glRotatef(angy,0.0,1.0,0.0);
        glRotatef(angz,0.0,0.0,1.0);
        glScalef(scale,scale,scale);

        //angulo inicial (tentar alterar para lookat depois)
        //glRotatef(-30,1,0,0);
        //glRotatef(-70,0,1,0);


        desenhaPikachu();

        //desenhapteste();

    glPopMatrix();

    desenhaQueijo();

    glutPostRedisplay();
    glutSwapBuffers();
}

//
//void desenhapteste(){
//    glColor3f(0,0,1);
//    glPushMatrix();
//        glTranslatef(0,-.7,0);
//        glutWireCone(.1,.3,30,30);
//        glTranslatef(0,0,.2);
//        glScalef(.2,.2,.2);
//        glColor3f(1,0,0);
//        glutSolidSphere(.2,30,30);
//    glPopMatrix();
//
//}

void desenhaPikachu()
{
    //corpo
        glPushMatrix(); //inicia operacoes no corpo
            glScalef(.7,.7,.7);

            //desenha corpo
            glColor3f(255,255,0);
            glPushMatrix();
                glScalef(1,.8,.7);
                glutSolidSphere(.5,50,50);
            glPopMatrix();

            //desenha listra 1
            glColor3f(.8,.6,0);
            glPushMatrix();
                glTranslatef(0,.02,0);
                glScalef(.5,.5,.5);
                glBegin(GL_TRIANGLES);
                    glVertex3f(0,.6,0);
                    glVertex3f(-.2,0,-.6);
                    glVertex3f(-.2,0,.6);
                glEnd();
            glPopMatrix();

            glColor3f(1,0,0);
            par1 = 1;
            desenhaPerninhas(-.3,-.2,.35,80.0,1.0,0,0);
            desenhaPerninhas(.3,-.2,-.15,100.0,1.0,0,0);

            par1 = 0;
            desenhaPerninhas(.3,-.2,.35,80.0,1.0,0);
            desenhaPerninhas(-.3,-.2,-.15,100.0,1.0,0,0);


/*
            //desenha perna frontal direita
            glColor3f(1,0,0);
            glPushMatrix();
                //glTranslatef(-.3,-.2,.3);
                //glRotatef(40,1,0,0);
                desenhaPerninhas(-.3,-.2,.3,40,0,0);
            glPopMatrix();

            //desenha perna traseira direita
            glColor3f(255,255,0);
            glPushMatrix();
                //glTranslatef(.3,-.2,.3);
                //glRotatef(40,1,0,0);
                desenhaPerninhas(.3,-.2,.3,40,0,0);
            glPopMatrix();

            //desenha perna frontal esquerda
            glColor3f(1,0,0);
            glPushMatrix();
                //glTranslatef(-.3,-.2,-.3);
                //glRotatef(140,1,0,0);
                desenhaPerninhas(-.3,-.2,-.3,140,1,0,0);
            glPopMatrix();

            //desenha perna traseira esquerda
            glColor3f(255,255,0);
            glPushMatrix();
                //glTranslatef(.3,-.2,-.3);
                //glRotatef(140,1,0,0);
                desenhaPerninhas(.3,-.2,-.3,140,1,0,0);
            glPopMatrix();
*/

        glPopMatrix();  //termina operacoes no corpo

        //cabeca
        glPushMatrix(); //inicia operacoes na cabeca
            glTranslatef(.1,0,0);
            desenhaCabeca();
        glPopMatrix();  //termina operacoes na cabeca
}

void desenhaPerninhas(double tx, double ty, double tz, double angle, double rx, double ry, double rz){
    //ombro
    glColor3f(255,255,0);
    glPushMatrix();
        glTranslatef(tx,ty+.2,tz-.1);
        glScalef(1,1.5,.9);
        glutSolidSphere(.1,50,50);
    glPopMatrix();

    glPushMatrix();

        if(par1 == 1) glTranslatef(translada_par1,0,0);
        else    glTranslatef(-translada_par1,0,0);
        //coxa
        glColor3f(255,255,0);
        glPushMatrix();
            glTranslatef(tx,ty+.1,tz-.1);
            if(par1 == 1)
            {
                glRotatef(angulo_coxa_fe,0,0,1);
            }
            else glRotatef(-angulo_coxa_fe,0,0,1);
            glScalef(1,1.5,1);
            glutSolidCube(.15);
        glPopMatrix();
        glColor3f(255,255,0);
        glPushMatrix();
            glTranslatef(tx,ty,tz-.1);
            glScalef(1,1,.9);
            glutSolidSphere(.1,50,50);
        glPopMatrix();

        glPushMatrix();

            //perna
            glColor3f(255,255,0);
            glPushMatrix();
                glTranslatef(tx,ty-.1,tz-.1);
                if(par1 == 1) glRotatef(angulo_perna_fe,0,0,1);
                else glRotatef(-angulo_perna_fe,0,0,1);
                glScalef(1,1.7,1);
                glutSolidCube(.1);
            glPopMatrix();

            //pe
            glColor3f(255,255,0);
            glPushMatrix();
                glTranslatef(tx-.05,ty-.2,tz-.1);
                glRotatef(50,0,0,1);
                glScalef(.7,.6,.7);
                glutSolidSphere(.1,50,50);
            glPopMatrix();

        glPopMatrix();

    glPopMatrix();
}

void desenhaCabeca(){
        //desenha cabeca
        glColor3f(255,255,0);
        glPushMatrix();
            glTranslatef(-.5,.07,0);
            glScalef(.5,.5,.5);
            glutSolidSphere(.5,50,50);
        glPopMatrix();

        //desenha olho direito
        glColor3f(0,0,0);
        glPushMatrix();
            glTranslatef(-.7,.07,.11);
            glScalef(.08,.08,.08);
            glutSolidSphere(.5,50,50);
        glPopMatrix();

        //desenha branco olho direito
        glColor3f(1,1,1);
        glPushMatrix();
            glTranslatef(-.73,.08,.11);
            glScalef(.03,.03,.03);
            glutSolidSphere(.5,50,50);
        glPopMatrix();


        //desenha olho esquerdo
        glColor3f(0,0,0);
        glPushMatrix();
            glTranslatef(-.7,.07,-.11);
            glScalef(.08,.08,.08);
            glutSolidSphere(.5,50,50);
        glPopMatrix();

        //desenha branco olho esquerdo
        glColor3f(1,1,1);
        glPushMatrix();
            glTranslatef(-.73,.08,-.11);
            glScalef(.03,.03,.03);
            glutSolidSphere(.5,50,50);
        glPopMatrix();

        //desenha nariz
        glColor3f(0,0,0);
        glPushMatrix();
            glTranslatef(-.75,0,0);
            glScalef(.03,.03,.03);
            glutSolidSphere(.3,50,50);
        glPopMatrix();

        //desenha bochecha direito
        glColor3f(1,0,0);
        glPushMatrix();
            glTranslatef(-.67,-.03,.13);
            glScalef(.05,.05,.05);
            glutSolidSphere(.7,50,50);
        glPopMatrix();

        //desenha bochecha direito
        glColor3f(1,0,0);
        glPushMatrix();
            glTranslatef(-.67,-.03,-.13);
            glScalef(.05,.05,.05);
            glutSolidSphere(.7,50,50);
        glPopMatrix();

        //desenha orelha direita
        glColor3f(255,255,0);
        glPushMatrix();
            glTranslatef(-.5,.2,.2);
            glRotatef(-40,1,0,0);
            glutWireCone(.05,.3,50,50);
        glPopMatrix();

        //desenha ponta orelha direita
        glColor3f(0,0,0);
        glPushMatrix();
            glTranslatef(-.5,.29,.3);
            glRotatef(-40,1,0,0);
            glutSolidCone(.037,.2,50,50);
        glPopMatrix();

        //desenha orelha esquerda
        glColor3f(255,255,0);
        glPushMatrix();
            glTranslatef(-.5,.2,-.2);
            glRotatef(-140,1,0,0);
            glutWireCone(.05,.3,50,50);
        glPopMatrix();

        //desenha ponta orelha esquerda
        glColor3f(0,0,0);
        glPushMatrix();
            glTranslatef(-.5,.29,-.3);
            glRotatef(-140,1,0,0);
            glutSolidCone(.037,.2,50,50);
        glPopMatrix();
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
        glVertex3f(tmp->c.qx,tmp->c.qy,0);
        tmp = tmp->prox;
    }
    glEnd();
}

void keyboard (unsigned char key, int x, int y)
{
    switch (key)
    {
    /*controla escala da cena*/
    case 'E':
        scale+=.02;
        break;
    case 'e':
        scale-=.02;
        break;

    /* controla rota›es da cena */
    case 'o':
        angx = 0;
        angy = 0;
        angz = 0;
        translada_x = -translada_x;
        translada_y = -translada_y;
        translada_z = -translada_z;
        break;
    case 'x':
        angx = (angx-10)%360;
        break;

    case 'X':
        angx = (10 + angx)%360;
        break;


    case 'y':
        angy = (angy-10)%360;
        break;

    case 'Y':
        angy = (10 + angy)%360;
        break;



    case 'z':
        angz = (angz-10)%360;
        break;


    case 'Z':
        angz += (10 + angz)%360;
        break;

    //posiciona camera
    case '1':
        gluLookAt(-1,0,0,0,0,0,0,1,0);
        break;

    case '2':
        gluLookAt(0,1,0,cx,cy,cz,0,1,0);
        break;

    case '3':
        //gluLookAt();
        break;

    case 'a':
        angulo_coxa_fe = angulo_perna_fe;
        angulo_perna_fe = angulo_perna_fe == 50?-50:50;
        translada_par1 = -translada_par1 ;
        break;

    /* sai do programa */
    case 27:
        exit(0);
        break;

    }
}

void moveRato()
{
    printf("procurando queijo\n");
    while(getFirst(f))  //vai recebendo o primeiro queijo da fila ate fila ficar vazia
    {
        printf("Movendo (%.3f,%.3f)\n",dx,dy);
        for(int i = 0; i < 20; i++)
        {
            cx+= dx/20;
            cy+= dy/20;
            cz+= dz/20;
            nx+= dx/20;
            ny+= dy/20;
            nz+= dz/20;

            translada_x+= dx/20;
            translada_y+= dy/20;

            //movimento das pernas
            angulo_coxa_fe = angulo_perna_fe;
            angulo_perna_fe = angulo_perna_fe == 50?-50:50;
            translada_par1 = -translada_par1 ;


            //translada_z+= dz/20;
            display();
            usleep(1000*50);
            glFlush();
        }
        removeNo(f);
    }
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
        printf("angulo: %f\n",toDegree(angz));
        angz+=toRadian(10);
        tmp_angle+=toRadian(10);
        display();
        usleep(1000*50);
        glFlush();
    }
    if(alfa < 0)
    {
        while(tmp_angle > alfa)
        {
            printf("angulo: %f\n",toDegree(angz));
            angz-=toRadian(10);
            tmp_angle-=toRadian(10);
            display();
            usleep(1000*50);
            glFlush();
        }

    }

}

void posicionaQueijo (int botao, int estado, int x, int y)
{
    switch(botao)
    {
    case GLUT_LEFT_BUTTON:
        if(estado == GLUT_DOWN)
        {
            //count++;

            novo_qx = ((float)x/((float)w/2.0))-1.0;
            novo_qy = (float) 1 - y/(h/2.0);
            novo_qz = 0;

            //limitacao da tela
//            if(novo_qx>.7)  novo_qx = .7;
//            else if(novo_qx<-.7) novo_qx = -.7;
//
//            if(novo_qy>.7)  novo_qy = .7;
//            else if(novo_qy<-.7) novo_qy = -.7;

            //Insere a posicao do queijo na Fila
            printf("Tentando inserir queijo na posicao (%.3f,%.3f, %.3f)\n",novo_qx,novo_qy,novo_qz);
            insereNo(f);
            exibeFila(f);

            rodaRato();

            //TODO - tem que fazer o rato mover aos poucos
            moveRato(); //leva o rato ate o queijo criado
        }
        break;
    case GLUT_RIGHT_BUTTON:
        //moveRato();
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    f = (Fila*)malloc(sizeof(Fila));
    iniciaFila(f);

    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DEPTH |GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (w, h);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Pikachu 3D");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(posicionaQueijo);
    glutMainLoop();
    return 0;
}

