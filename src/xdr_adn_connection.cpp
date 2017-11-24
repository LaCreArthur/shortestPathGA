#include "xdr_adn_connection.h"
#define ADNV_MAX_SIZE 100

void ADNV_new(Adn_vector *adnv, int num, int max_size)
{
    adnv->index_read=0;
    adnv->index_write=0;
    adnv->num_village=num;
    adnv->size=0;
    adnv->adns=(Adn **)malloc(max_size*sizeof(Adn *));
}

void ADNV_insert(Adn_vector *adnv, Adn *adn, int num, unsigned int max_size)
{
    adnv->num_village=num;//témoigne du numéro du village ayant inséré en dernier
    adnv->adns[adnv->index_write]=adn;
    if(++adnv->size>max_size)
    {
        adnv->size=max_size;
        adnv->index_write=(adnv->index_write+1)%(adnv->size);
    }
    else
        adnv->index_write++;
}

void ADNV_next_read(Adn_vector *adnv)
{
    adnv->index_read=(adnv->index_read+1)%adnv->size;
}

Adn *ADNV_read(Adn_vector *adnv)
{
    return adnv->adns[adnv->index_read];
}

void ADNV_new_global(Adn_vector *global) //création du bureau des immigrés
{
    global->index_read=0;
    global->index_write=0;
    global->size=0;
    global->num_village=0;
    global->adns=(Adn **)malloc(ADNV_MAX_SIZE*sizeof(Adn *));
    int i;
    for(i=0;i<ADNV_MAX_SIZE;i++)
        global->adns[i]=(Adn *)malloc(sizeof(Adn));
}

void ADNV_append(Adn_vector *dest, Adn_vector *src) //rajout des adns de src au bureau des immigrés
{
    unsigned int i;
    if(dest->size+src->size > ADNV_MAX_SIZE)
        dest->size=ADNV_MAX_SIZE;
    else
        dest->size=dest->size+src->size;
    for(i=0;i<src->size;i++)
    {
        memcpy(dest->adns[dest->index_write],src->adns[i],sizeof(Adn));
        dest->index_write=(dest->index_write+1)%ADNV_MAX_SIZE;
    }
    dest->num_village=src->num_village;//cette instruction annonce que le dernier village ayant écrit était num_village.
}

Adn_vector ADNV_give_adns(Adn_vector *src, int num, int num_village) //distribution des num immigrés en tête de lecture.
{//A utiliser pour l'appel statique. Précondition : on donne autant d'immigrés qu'on en reçoit à CHAQUE appel.
    int i;
    int special_read_head=src->index_read;
    Adn_vector ret;
    ret.num_village=num_village;
    ret.size=num;
    ret.index_read=0;
    ret.index_write=0;
    ret.adns=(Adn **)malloc(num*sizeof(Adn *));
    for(i=0;i<num;i++)
    {
        ret.adns[i]=(Adn *)malloc(sizeof(Adn));
        memcpy(ret.adns[i],src->adns[src->index_read],sizeof(Adn));
        src->index_read=(src->index_read+1)%ADNV_MAX_SIZE;
    }
    if(ret.num_village==src->num_village)
        src->index_read=special_read_head;//cela signifie qu'on a renvoyé des immigrés que le village a écrit.
        //Tant pis, on était obligé de les renvoyer de toute façon ; cependant on ne bougera pas la tête de lecture.
        //Ainsi nous auront un clone de ces immigrés pour les autres villages.
    return ret;
}

bool_t xdr_adn(XDR *xdrs, Adn *individu)
{
    int i;
    bool_t ok=1;
    for(i=0;i<500 && ok;i++)
        ok=xdr_u_char(xdrs,&individu->adn[i]);
    if(ok)
        ok=xdr_int(xdrs,&individu->nb_pas);
    if(ok)
        return xdr_int(xdrs,&individu->score);
    else
        return ok;
}

bool_t xdr_adn_vector(XDR *xdrs, Adn_vector *adnv)
{
    return xdr_array(xdrs,
              (char**)(adnv->adns),
              &adnv->size,
              100,
              sizeof(Adn),
              (xdrproc_t) xdr_adn );
}

bool_t xdr_info_connect(XDR *xdrs, info_connect *ic)
{
    bool_t ok;
    ok=xdr_int(xdrs, &ic->nb_villages);
    if (ok)
        ok=xdr_int(xdrs, &ic->pc_mutations);
    if (ok)
        ok=xdr_int(xdrs, &ic->nb_mutations);
    if(ok)
        ok=xdr_int(xdrs, &ic->nb_individus);
    if(ok)
        ok=xdr_int(xdrs, &ic->num_village);
    if(ok)
        ok=xdr_int(xdrs, &ic->gen_par_mig);
    if(ok)
        ok=xdr_int(xdrs, &ic->pas);
    if(ok)
        ok=xdr_int(xdrs, &ic->taux_migration);
    if(ok)
        ok=xdr_int(xdrs, &ic->convergence);
    if(ok)
        ok=xdr_int(xdrs, &ic->nb_mutations);
    return ok;
}

bool_t xdr_position(XDR *xdrs, position *p)
{
    return xdr_double(xdrs, &p->x) && xdr_double(xdrs, &p->y);
}

bool_t xdr_cercle(XDR *xdrs, cercle *c)
{
    bool_t ok;
    ok=xdr_position(xdrs, &c->p);
    if (ok)
        ok=xdr_double(xdrs, &c->rayon);
    return ok;
}

bool_t xdr_map(XDR *xdrs, map *m)
{
    bool_t ok;
    ok=xdr_int(xdrs, &m->width);
    if (ok)
        ok=xdr_int(xdrs, &m->height);
    if (ok)
        return ok=xdr_array(xdrs,
              (char **)(&(m->obstacles)),
              &m->nb_obstacles,
              100,
              sizeof(cercle),
              (xdrproc_t) xdr_cercle );
    else return 0;
}

bool_t xdr_connect(XDR *xdrs, main_connect *mc)
{
    return xdr_map(xdrs,&mc->map_principale) && xdr_info_connect(xdrs,&mc->informations);
}

