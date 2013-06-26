#include <iostream>
#include <fstream>
#include "struttura_dati.h"


/** Inizializzazione della coda
 *
 * La funzione prende in ingresso una coda e la inizializza a 0 elementi.
 */
void inizializza_coda(queue coda){
	coda.n_elementi=0;
}

/** Conta le occorrenze di ogni lettera.
 *
 * Prende in ingresso il file sorgente e conta le occorrenze di tutte le
 * lettere.
 */
void conta_occorrenze(char sorgente[], queue coda){

}


bool comprimi (char sorgente[], char destinazione[]){
	queue coda;
	inizializza_coda(coda);

	conta_occorrenze(sorgente, coda);
pulisci_coda(coda);
int num_caratteri=coda.n_elementi;
crea_albero(coda);
scrivi_file_compresso(destinazione, coda, num_caratteri); //contiene esplorazione dell'albero, scrittura del file compresso, etc..
return true;
}
