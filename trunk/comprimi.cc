#include <iostream>
#include <fstream>
#include "struttura_dati.h"

bool comprimi (char sorgente[], char destinazione[])
{
queue coda;
coda.n_elementi=0;
conta_occorrenze(sorgente, coda);
pulisci_coda(coda);
int num_caratteri=coda.n_elementi;
crea_albero(coda);
scrivi_file_compresso(destinazione, coda, num_caratteri); //contiene esplorazione dell'albero, scrittura del file compresso, etc..
return true;
}
