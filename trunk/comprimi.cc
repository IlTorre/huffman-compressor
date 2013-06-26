#include <iostream>
#include <fstream>
#include "struttura_dati.h"


/** Inizializzazione della coda
 *
 * La funzione prende in ingresso una coda e la inizializza a 0 elementi.
 */
void inizializza_coda(queue &coda){
	for (int i=1; i<CARATTERI_ASCII;i++){
		coda.elemento[i]=new nodo_t;
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
	coda.n_elementi=CARATTERI_ASCII-1	;
	return true;
}


void scambio(pnode &a,pnode &b)
{
pnode p=a;
a=b;
b=p;
}


void MinHeapify(queue &coda, int j, int n)
{
int k=j;
if ((2*j+1<=n) && (coda.elemento[2*j+1]->occorrenze<coda.elemento[k]->occorrenze))
	k=2*j+1;
if (2*j<=n && coda.elemento[2*j]->occorrenze<coda.elemento[k]->occorrenze)
	k=2*j;
if (k!=j)
	{
	scambio (coda.elemento[j],coda.elemento[k]);
	MinHeapify(coda, k, n);
	}
}


void BuildMinHeap(queue &coda)
{
for (int i=coda.n_elementi/2;i>=1;i--)
	MinHeapify(coda,i,coda.n_elementi);
}


void HeapSort(queue &coda)
{
for (int i=coda.n_elementi;i>1;i--)
	{
	scambio(coda.elemento[1],coda.elemento[i]);
	MinHeapify(coda,1,i-1);
	}
}

void pulisci_coda(queue &coda)
{
BuildMinHeap(coda);
HeapSort(coda);
while(coda.elemento[coda.n_elementi]->occorrenze==0)
	coda.n_elementi--;
}

bool comprimi (char sorgente[], char destinazione[]){
	queue coda;
	inizializza_coda(coda);
	conta_occorrenze(sorgente, coda);
	pulisci_coda(coda);
	for(int i=1;i<=coda.n_elementi;i++){
		cout<<coda.elemento[i]->carattere<<"\t"<<coda.elemento[i]->occorrenze<<endl;
	}
/*pulisci_coda(coda); FATTO
int num_caratteri=coda.n_elementi;
crea_albero(coda);
scrivi_file_compresso(destinazione, coda, num_caratteri); //contiene esplorazione dell'albero, scrittura del file compresso, etc..*/
return true;
}
