#ifndef VECTOR_H
#define VECTOR_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vector.h"

#define VNULL V_new(INFINITY,INFINITY,INFINITY)

class Vector
{
public:
    Vector(double x, double y);
    ~Vector();

    double x, y;

    double RadToDeg(double x);

    void V_print(char *message);
    // affiche à l'écran les coordonnées de v + un message (debug)

    void V_add(Vector v);
    // retourne v1+v2

    void V_substract(Vector v);
    // retourne v1-v2

    void V_multiply(double lambda);
    // retourne lambda * v
};

#endif // VECTOR_H
