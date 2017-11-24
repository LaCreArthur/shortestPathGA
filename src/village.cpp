#include "village.h"
#include <iostream>

void print_vector(std::vector<Individu *> *v)
{
    for(std::vector<Individu *>::const_iterator i = v->begin(); i != v->end(); ++i)
        std::cout << "Un des adn a obtenu un score de " << (*i)->adnStruct.score << '\n';
}

Village::Village(main_connect connection, Rpc_client *rpcHandler, GLWidget* debugWindow)
{
    this->rpcHandler=rpcHandler;
    this->debugWindow=debugWindow;
    this->connection=connection;
    this->genRestante=0;
    individus_notes=NULL;
    les_meilleurs=NULL;
    individus = new Individu[connection.informations.nb_individus];
    Individu::pas = connection.informations.pas;

    for (int i = 0; i<connection.informations.nb_individus; i++){
        individus[i].initSeDeplacer();
    }
}

Village::~Village()
{
    //delete individus;
}

void Village::test_and_evaluate_all()
{
    delete individus_notes;
    individus_notes=new std::multimap<int, Individu *, std::greater_equal<int> >();
    for(int i=0; i<connection.informations.nb_individus; i++)
    {
        individus[i].x=0;
        individus[i].y=0;
        individus[i].adnStruct.nb_pas=0;
        bool test = false;
        for (int j=0; j < MAX_ADN; j++){
            individus[i].adnStruct.nb_pas++;
            if (test) break;

            switch(individus[i].adnStruct.adn[j]){
                case 0:
                    individus[i].y += individus[i].pas;
                    if(individus[i].y>400) test = true;
                    break;
                case 1:
                    individus[i].x+=individus[i].pas;
                    individus[i].y+=individus[i].pas;
                    if(individus[i].x>400 || individus[i].y>400) test = true;
                    break;
                case 2:
                    individus[i].x+=individus[i].pas;
                    if(individus[i].x>400) test = true;
                    break;
                case 3:
                    individus[i].x+=individus[i].pas;
                    individus[i].y-=individus[i].pas;
                    if(individus[i].y<0 || individus[i].x>400) test = true;
                    break;
                case 4:
                    individus[i].y-=individus[i].pas;
                    if(individus[i].y<0) test = true;
                    break;
                case 5:
                    individus[i].x-=individus[i].pas;
                    individus[i].y-=individus[i].pas;
                    if(individus[i].y<0 || individus[i].x<0) test = true;
                    break;
                case 6:
                    individus[i].x-=individus[i].pas;
                    if(individus[i].x<0) test = true;
                    break;
                case 7:
                    individus[i].x-=individus[i].pas;
                    individus[i].y+=individus[i].pas;
                   if(individus[i].x<0 || individus[i].y>400) test = true;
                    break;
                default:
                    break;
                }
        if (!test)
            test = this->circle_colision(individus[i].x,individus[i].y);
        }
        individus[i].eval_score();
        individus_notes->insert(std::pair<int, Individu *>(individus[i].adnStruct.score,&individus[i]));
    }
}

bool Village::circle_colision(int x, int y) {
    bool test = false;
    for (unsigned int i=0; i < connection.map_principale.nb_obstacles; i++){
        position c = connection.map_principale.obstacles[i].p;
        double rayon = connection.map_principale.obstacles[i].rayon;
        if ((((x-c.x)*(x-c.x)) + ((y-c.y)*((y-c.y)))) < rayon*rayon) {
            test = true;
           // std::cout << "collision \n";
        }
    }
    return test;
}

Individu *Village::best()
{
    Individu *best;
    if(individus_notes->size())
    {
        best=individus_notes->begin()->second;
        individus_notes->erase(individus_notes->begin());
    }
    else
        best=NULL;
    return best;
}

void Village::reproduire(int frequence_croisement_special)
{
    int mut = 0;
    int nb_mut = connection.informations.pc_mutations * connection.informations.nb_individus / 100;
    int special_parent;
    int affine=rand() % 20 + 1;
    std::vector<Individu *>::iterator it3Best;
    std::vector<Individu *>::iterator it;
    std::multimap<int, Individu *, std::greater_equal<int> >::iterator itv=individus_notes->begin();
    Individu special;

    for(it3Best=les_meilleurs->begin(); it3Best!=les_meilleurs->end(); ++it3Best)
    {    // Reproduire les 4 meilleurs avec tous le monde (en tout cas jusqu'à ce qu'on puisse plus les prendre, les autres avortent
        for(it=les_meilleurs->begin(); it!=les_meilleurs->end() ; ++it)
        {
            if(it3Best!=it)
            {
                if (nb_mut > 0)
                {
                    mut = rand() % 2;
                    if (mut == 1) nb_mut--;
                }
                special_parent=rand()%frequence_croisement_special;
                if(connection.informations.convergence && !special_parent) //special_parent==0
                {
                    special.outsiderIndividual(affine);
                    *itv->second = (*it3Best)->reproduction(special, mut, connection.informations.nb_mutations) ;
                }
                else
                    *itv->second = (*it3Best)->reproduction(**it, mut, connection.informations.nb_mutations) ;
                itv++;
                if(itv==individus_notes->end())
                    break;
            }
        }
        if(itv==individus_notes->end())
            break;
    }
}

void Village::use_map()
{
    int stop=connection.informations.nb_individus/4;
    int i;
    delete les_meilleurs;
    les_meilleurs= new std::vector<Individu *>();
    for(i=0; i<stop; i++)
    {
        les_meilleurs->push_back(best());
    }
    if(genRestante == 0)
    {
        migration(); //effectue un petit échange d'immigrés. ;)
        genRestante = rand() % connection.informations.gen_par_mig + 1;
    }
}

void Village::migration()
{
    static Adn_vector adnv;
    if(!rpcHandler)
        return;
    static int max_size=(connection.informations.nb_individus*connection.informations.taux_migration)/100;
    ADNV_new(&adnv, connection.informations.num_village, max_size);
    std::vector<Individu *>::iterator it=les_meilleurs->begin();
    for(int i=0 ; i<max_size; i++)
    {
        ADNV_insert(&adnv,&(*it)->adnStruct,connection.informations.num_village,max_size);
        it++;
    }
    if(rpcHandler->callTrade(&adnv))// !=0
        fprintf(stderr,"Attention : RPC a eu un probleme\n");
    free(adnv.adns);
}

bool Village::detectionStable(int tousLesNTours)
{
    static int sumBest=0;
    static int doVerif=1;
    if(!(doVerif%tousLesNTours))
    {
        int oldSum=sumBest;
        for (std::vector<Individu *>::iterator it=les_meilleurs->begin();it!=les_meilleurs->end();it++)
            sumBest+=(*it)->adnStruct.score;
        if(oldSum==sumBest)
            return true;
    }
    return false;
}

void Village::mainLoop(Adn_vector *adnv)
{
    Adn *best;
    int nb_gen=0;
    int quart=connection.informations.nb_individus/4;
    while(!detectionStable(50) && nb_gen<MAX_GEN)
    {
        test_and_evaluate_all();
        use_map();
        reproduire(quart);
        genRestante--;
        nb_gen++;
        if(adnv && les_meilleurs && debugWindow)
        {
            std::vector<Individu *>::iterator it=les_meilleurs->begin();
            best=&(*it)->adnStruct;
            ADNV_insert(adnv,best,connection.informations.num_village,les_meilleurs->size());
            debugWindow->repaint();
        }
    }
    delete individus_notes;
    delete les_meilleurs;
}
