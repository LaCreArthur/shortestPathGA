#ifndef INDIVIDU_H
#define INDIVIDU_H

#include "vector.h"
#include "xdr_adn_connection.h"

#define MAX_ADN 500 // taille de l'ADN
#define SCORE 1000  //sc
#define _e 10
#define checkEps_bool(value) ((value)>_e || (value)<-_e)
#define square(X) ((X)*(X))

class Individu
{
public:
    Individu();
    ~Individu();

    Adn adnStruct;

    int x,y;
    static int pas;

    Individu reproduction(Individu parent2, int mut, int nbGenAMut) ;
    void mutationIndividus(int nb_mutations);
    void eval_score();

    void outsiderIndividual(int x);
    void initSeDeplacer();
};

#endif // INDIVIDU_H

