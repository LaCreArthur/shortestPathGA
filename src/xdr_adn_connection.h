#ifndef XDR_ADN_CONNECTION
#define XDR_ADN_CONNECTION

#include <rpc/types.h>
#include <rpc/xdr.h>
#include <rpc/rpc.h>
#include <stdlib.h>

#define PROGNUM 0x20000010
#define VERSNUM 1
#define PROCFIRST 1
#define PROCSECOND 2


typedef struct {int nb_villages; int nb_mutations; int pc_mutations; int nb_individus; int taux_migration; int num_village; int gen_par_mig; int pas; int convergence; int nb_generations;} info_connect;
typedef struct {double x; double y;} position;
typedef struct {position p; double rayon;} cercle;
typedef struct {int width; int height; unsigned int nb_obstacles; cercle *obstacles;} map;
typedef struct {map map_principale; info_connect informations;} main_connect;

typedef struct {int score; int nb_pas; unsigned char adn[500];} Adn;
typedef struct {int index_write; int index_read; unsigned int size; int num_village; Adn **adns;} Adn_vector;

void ADNV_new(Adn_vector *adnv, int num, int max_size);
void ADNV_new_global(Adn_vector *global);
void ADNV_next_read(Adn_vector *adnv);
void ADNV_insert(Adn_vector *adnv, Adn *adn, int num, unsigned int max_size);
Adn *ADNV_read(Adn_vector *adnv);
void ADNV_append(Adn_vector *dest, Adn_vector *src);
Adn_vector ADNV_give_adns(Adn_vector *src, int num, int num_village);

bool_t xdr_adn(XDR *xdrs, Adn *individu);
bool_t xdr_adn_vector(XDR *xdrs, Adn_vector *adns);
bool_t xdr_info_connect(XDR *xdrs, info_connect *ic);
bool_t xdr_position(XDR *xdrs, position *p);
bool_t xdr_map(XDR *xdrs, map *m);
bool_t xdr_connect(XDR *xdrs, main_connect *mc);

#endif // XDR_ADN_CONNECTION

