#include "vector.h"

Vector::Vector(double x, double y)
{
    this->x = x;
    this->y = y;
}

/*----------------------------*/
double Vector::RadToDeg(double x)
{
    double Pi = 3.14159265;
    return x / Pi * 180;
}

/*----------------------------*/
void Vector::V_add(Vector v)
{
    this->x += v.x;
    this->y += v.y;
}

/*----------------------------*/
void Vector::V_substract(Vector v)
{
    this->x -= v.x;
    this->y -= v.y;
}

/*----------------------------*/
void Vector::V_multiply(double lambda)
{
    this->x *= lambda;
    this->y *= lambda;
}

//------------------------------------------------
// a des fin de debug

void Vector::V_print(char *message)
{
    fprintf(stderr,"%s : %f %f\n",message, this->x,this->y);
}

Vector::~Vector()
{

}

