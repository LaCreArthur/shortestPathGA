#include <QtGui/QMouseEvent>
#include <GL/glut.h>
#include <GL/glu.h>
#include <QGLWidget>
#include "glwidget.h"
#include "mainwindow.h"
#include <iostream>

using namespace std;

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent) {
    setMouseTracking(true);
    this->setFixedSize(400, 400);
    nbcircles = 5;
    minradius = 5;
    maxradius = 15;
    cercles = (cercle *) malloc(20 * sizeof(cercle));
    drawRandomCircles(cercles, nbcircles, minradius, maxradius);
}

void GLWidget::initializeGL() {
    glViewport(0, 0, this->width(), this->height());
    glClearColor(0, 0, 0, 1);
}

void GLWidget::resizeGL(int w, int h) { // normalement useless
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, 0, h,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int GLWidget::draw_route(Adn *i){
    MainWindow *p;
    p=(MainWindow *)parent();
    int x=0, y=0;
    glColor3d(1,1,1);
    glBegin(GL_LINES);
    for (int j=0; j < i->nb_pas; j++)
    {
        switch(i->adn[j]) {
        case 0:
            glVertex2f(x,y);
            y += p->p_main_connexion->informations.pas;
            glVertex2f(x,y);
            if(y>400) return -1;
            break;
        case 1:
            glVertex2f(x,y);
            x+= p->p_main_connexion->informations.pas;;
            y+= p->p_main_connexion->informations.pas;
            glVertex2f(x,y);
            if(x>400 || y>400) return -1;
            break;
        case 2:
            glVertex2f(x,y);
            x+= p->p_main_connexion->informations.pas;
            glVertex2f(x,y);
            if(x>400) return -1;
            break;
        case 3:
            glVertex2f(x,y);
            x+= p->p_main_connexion->informations.pas;
            y-= p->p_main_connexion->informations.pas;
            glVertex2f(x,y);

            if(x>400 || y<0) return -1;
            break;
        case 4:
            glVertex2f(x,y);
            y-= p->p_main_connexion->informations.pas;
            glVertex2f(x,y);
            if(y<0) return -1;
            break;
        case 5:
            glVertex2f(x,y);
            x-= p->p_main_connexion->informations.pas;
            y-= p->p_main_connexion->informations.pas;
            glVertex2f(x,y);
            if(y<0 || x<0) return -1;
            break;
        case 6:
            glVertex2f(x,y);
            x-= p->p_main_connexion->informations.pas;
            glVertex2f(x,y);
            if(x<0) return -1;
            break;
        case 7:
            glVertex2f(x,y);
            x-= p->p_main_connexion->informations.pas;
            y+= p->p_main_connexion->informations.pas;
            glVertex2f(x,y);
            if(x<0 || y>400) return -1;
            break;
        default:
            cout << "S'est prit un mur ahah ?\n";
            return 1;
        }
    }
    glEnd();
    return 0;
}

void GLWidget::drawCircle(cercle c){
   glColor3d(1,0,0);
    glBegin(GL_LINE_LOOP);
   int i;
   double DEG2RAD = 3.14159/180;
   for (i=0; i < 360; i++)
   {
      float degInRad = i*DEG2RAD;
      Vector v(cos(degInRad)*c.rayon,sin(degInRad)*c.rayon);
      Vector t(c.p.x,c.p.y);
      v.V_add(t);
      glVertex2f(v.x, v.y);
   }
   glEnd();
}

void GLWidget::drawRandomCircles(cercle *v_cercle, int nb_circles, int min, int max){
    double x,y;
    int r;
    for(int i=0; i < nb_circles; i++) {
        while ((x = rand() % 400) < 30 || x > 370)
            x = rand() % 400;
        while ((y = rand() % 400) < 30 || y > 370)
            y = rand() % 400;
        r = rand() % max + min;
        v_cercle[i].p.x=x;
        v_cercle[i].p.y=y;
        v_cercle[i].rayon=r;
    }
}

void GLWidget::drawQuad(Vector p1, Vector p2, Vector p3, Vector p4)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_QUADS);
    glVertex2f(p1.x,p1.y);
    glVertex2f(p2.x,p2.y);
    glVertex2f(p3.x,p3.y);
    glVertex2f(p4.x,p4.y);
    glEnd();
}

void GLWidget::paintGL() {

    MainWindow *p;
    glClear(GL_COLOR_BUFFER_BIT);

    // dessiner ici !
    // ....

    for (int i=0; i < nbcircles; i++) {
        drawCircle(cercles[i]);
    }

    p=(MainWindow *)parent();
    for(unsigned int k=0;k<p->p_bureau->size;k++)
    {
        draw_route(ADNV_read(p->p_bureau));
        ADNV_next_read(p->p_bureau);
    }


    // Dessin du depart
    Vector e1 (0,0);
    Vector e2 (20,0);
    Vector e3 (20,20);
    Vector e4 (0,20);
    glColor3d(0.5,1,0.5);
    drawQuad(e1,e2,e3,e4);

    glColor3d(0,0,0);
    QString dep = "Dep";
    glRasterPos2f(0,5);
    for(int j = 0; j < dep.size(); ++j){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, dep.toStdString()[j]);
    }

    // Dessin de l'arrivee
    Vector f1 (400,400);
    Vector f2 (380,400);
    Vector f3 (380,380);
    Vector f4 (400,380);
    glColor3d(0.5,0.5,1);
    drawQuad(f1,f2,f3,f4);

    glColor3d(0,0,0);
    QString arr = "Arr";
    glRasterPos2f(381,385);
    for(int j = 0; j < arr.size(); ++j){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, arr.toStdString()[j]);
    }
}

