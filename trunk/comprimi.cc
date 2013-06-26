#include <iostream>
#include <fstream>
#include "struttura_dati.h"


/** Inizializzazione della coda
 *
 * La funzione prende in ingresso una coda e la inizializza a 0 elementi.
 */
void inizializza_coda(queue &coda){
	coda.elemento=new pnode [CARATTERI_ASCII];
	for (int i=1; i<CARATTERI_ASCII;i++){
		coda.elemento[i]->occorrenze=0;
		coda.elemento[i]->carattere=static_cast<char>(i-1);
		}
	coda.n_elementi=0;
}

/** Conta le occorrenze di ogni lettera.
 *
 * Prende in ingresso il file sorgente e conta le occorrenze di tutte le
 * lettere. La funzione restituisce un valore booleano che segnala se
 * l'operazione è andata a buon fine.
 */
bool conta_occorrenze(char sorgente[], queue &coda){
	char car;
	ifstream f (sorgente);
	if (!f) 
		return false;
	while(f.read(reinterpret_cast<char *>(&car), sizeof (car)))
		coda.elemento[static_cast<int>(car)+1]->occorrenze++;
	coda.n_elementi=CARATTERI_ASCII-1;
	return true;
}


bool comprimi (char sorgente[], char destinazione[]){
	queue coda;

	cout<<"provo a inizializzare la coda"<<endl;
	inizializza_coda(coda);
	cout<<"provo a contare"<<endl;
	conta_occorrenze(sorgente, coda);
	cout<<"provo a stampare"<<endl;
	for(int i=1;i<coda.n_elementi;i++){
		cout<<coda.elemento[i]->carattere<<"\t"<<coda.elemento[i]->occorrenze<<endl;
	}
/*pulisci_coda(coda);
int num_caratteri=coda.n_elementi;
crea_albero(coda);
scrivi_file_compresso(destinazione, coda, num_caratteri); //contiene esplorazione dell'albero, scrittura del file compresso, etc..*/
return true;
}
