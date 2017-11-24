#ifndef RPC_CLIENT_H
#define RPC_CLIENT_H
#include "xdr_adn_connection.h"

class Rpc_client {

public:

    Rpc_client(char *host);
    char *host;

    int callConnection(main_connect *mc);
    int callTrade(Adn_vector *adnv);
};

#endif // RPC_CLIENT_H
