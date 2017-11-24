#include "rpc_client.h"

Rpc_client::Rpc_client(char *host)
{
    this->host=host;
}

int Rpc_client::callConnection(main_connect *mc)
{
    return callrpc(
             /* host */ host,
             /* prognum */ PROGNUM,
             /* versnum */ VERSNUM,
             /* procnum */ PROCFIRST,
             /* encodage argument */ (xdrproc_t) xdr_void,
             /* argument */ NULL,
             /* decodage retour */ (xdrproc_t) xdr_connect,
             /* retour de la fonction distante */(char *)mc);
}

int Rpc_client::callTrade(Adn_vector *adnv)
{
    return callrpc(
             /* host */ host,
             /* prognum */ PROGNUM,
             /* versnum */ VERSNUM,
             /* procnum */ PROCSECOND,
             /* encodage argument */ (xdrproc_t) xdr_adn_vector,
             /* argument */ adnv,
             /* decodage retour */ (xdrproc_t) xdr_adn_vector,
             /* retour de la fonction distante */(char *)adnv);
}
