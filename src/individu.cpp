#include "individu.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std ;

int Individu::pas = 1;

Individu::Individu()
{
    x=0;
    y=0;
    adnStruct.nb_pas=0;
}

Individu::~Individu()
{}

Individu Individu::reproduction(Individu parent2, int mut, int nbGenAMut)
{
    // on fait un crossing-Over entre 25% et 75% de l'ADN, choisie aleatoirement
    int min = MAX_ADN * 0.10 ;
    int max = MAX_ADN * 0.90 ;
    int dominant=rand()%2;
    int indiceCoupure = rand() % max + min ;

    Individu fils;

    for(int i=0 ; i<MAX_ADN ; i++)
    {
        if(i <= indiceCoupure)
        {
            if(dominant)
                fils.adnStruct.adn[i] = this->adnStruct.adn[i];
            else
                fils.adnStruct.adn[i] = parent2.adnStruct.adn[i];
        } // copie du brain du père
        else
        {
            if(dominant)
                fils.adnStruct.adn[i] = parent2.adnStruct.adn[i];
            else
                fils.adnStruct.adn[i] = this->adnStruct.adn[i];
        } // arrivée à la coupure , on copie sur la deuxieme moitié, la deuxieme moitié du brain de la mère
    }
    if (mut == 1) {
        fils.mutationIndividus(nbGenAMut);
    }
    return fils;
}

void Individu::mutationIndividus(int nb_mutations)
{
    for(int i=0 ; i < nb_mutations ; i++) // pour chaque mutation de l'ADN
    {
        int position = rand() % MAX_ADN; // le choix du maillon à muter
        this->adnStruct.adn[position] = rand() % 8; // mutation
    }
}


void Individu::initSeDeplacer(){

    for(int i=0 ; i< MAX_ADN ; i++)
        this->adnStruct.adn[i] = rand() %8 ;
}

void Individu::outsiderIndividual(int x){
    int signe = 1;
    int choix;
    double s;
    if(x)
        choix = rand() %3 + 1;
    else choix = 4;

    for(int i=0 ; i< MAX_ADN ; i++)
    {
        switch (choix) {
        case 1: // affine > fx=x
            if ((fmod(i,x)) == 0) this->adnStruct.adn[i]=1;
            else this->adnStruct.adn[i] = 0;

            break;

        case 2: // affine < fx=x
            if ((fmod(i,x)) == 0) this->adnStruct.adn[i]=1;
            else this->adnStruct.adn[i] = 2;

            break;
        case 3: //sinusoide
            s = sin((double)(i+1)/x);
            if (s > -0.8 && s < 0.8 && signe) {
                if (i > x && i%2 == 0) this->adnStruct.adn[i]=7;
                else this->adnStruct.adn[i]=0;
            }
            else if (s >= 0.8) {
                signe = 0;
                this->adnStruct.adn[i]=1;
            }
            else if (s > -0.8 && s < 0.8 && !signe) {
                if (i > x && i%2 == 0) this->adnStruct.adn[i]=3;
                else this->adnStruct.adn[i]=2;
            }
            else if (s <= -0.8) {
                signe = 1;
                this->adnStruct.adn[i] = 1;
            }
            else {
                this->adnStruct.adn[i]=0;
            }
            break;

        default: //full random
            this->adnStruct.adn[i] = rand() %8 ;
            break;

        }

    }
}

void Individu::eval_score(){
    double dist=0;
    int result=0;
    dist = sqrt(powf(this->x - 400,2)+powf(this->y - 400,2));
    result=-dist-((this->adnStruct.nb_pas)/2);
    this->adnStruct.score=result;
}
