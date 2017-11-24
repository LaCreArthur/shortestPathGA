#ifndef _GLWIDGET_H
#define _GLWIDGET_H

#include <QtOpenGL/QGLWidget>
#include "vector.h"
#include <GL/glut.h>
#include <cmath>
#include <QVector>
#include "individu.h"
#include "xdr_adn_connection.h"

class GLWidget : public QGLWidget {

public:
    GLWidget(QWidget *parent = NULL);
    QVector < Vector* > vect_circles;
    QVector < int > vect_radius;
    int nbcircles, minradius, maxradius;
    cercle *cercles;
    Adn *best_adn;

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void drawQuad(Vector p1, Vector p2, Vector p3, Vector p4);
    void drawCircle(cercle c);
    void drawRandomCircles(cercle *v_cercle, int nb_circles, int min, int max);
    int draw_route(Individu *i);
    int draw_route(Adn *i);
};

#endif  /* _GLWIDGET_H */
