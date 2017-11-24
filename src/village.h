#ifndef VILLAGE_H
#define VILLAGE_H

#include "xdr_adn_connection.h"
#include "individu.h"
#include "glwidget.h"
#include "rpc_client.h"

#include <unistd.h>
#include <map>
#include <vector>

void print_vector(std::vector<Individu *> *v);

#define MAX_GEN 5000
class Village
{
public:

// Constructeur et Destructeur

    Village(main_connect connection, Rpc_client *rpcHandler, GLWidget* debugWindow);
    ~Village();

    int nbreGeneration ; // s'incremente du debut du programme jusqu'a trouver la Sortie
    int genRestante;

    main_connect connection; //infos de connexion + map
    std::multimap<int, Individu *, std::greater_equal<int> > *individus_notes;
    std::vector<Individu *> *les_meilleurs;
    Individu *individus;
    Rpc_client *rpcHandler;
    GLWidget *debugWindow;

    void test_and_evaluate_all();
    Individu *best();
    bool detectionStable(int tousLesNTours);
    void use_map();
    bool circle_colision(int x, int y);

    void reproduire(int frequence_croisement_special);
    void migration() ;
    void mainLoop(Adn_vector *adnv);
};

#endif // VILLAGE_H
