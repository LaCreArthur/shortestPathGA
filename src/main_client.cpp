#include "village.h"

int main (int argc, char **argv) {
	if (argc != 2) 
  	{
		printf("Usage: %s machine_serveur\n",argv[0]); 
		exit(0); 
  	}

    bool_t stat;
  	Rpc_client rpcHandler(argv[1]);
	main_connect connection;
	stat=rpcHandler.callConnection(&connection);
    printf("Coherence des donnees ?\n\t%d, %d, %d, %d, %lf, %d\n",connection.informations.convergence,connection.informations.gen_par_mig, connection.informations.nb_mutations,connection.map_principale.height,connection.map_principale.obstacles[0].rayon,connection.informations.num_village);

    if (stat)
    {
        fprintf(stderr, "Echec de l'appel distant\n");
		exit(EXIT_FAILURE);
    }
	Village v(connection,&rpcHandler,NULL);
    v.mainLoop(NULL);
	printf("Un des clients a fini\n");

  return 0;
}

