#include <iostream>
#include <fstream>
#include "struttura_dati.h"


/** Inizializzazione della coda
 *
 * La funzione prende in ingresso una coda e la inizializza a 0 elementi.
 * La funzione inoltre azzera tutte le occorrenze
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


/** Scambia due nodi.
 *
 * La funzione prende in ingresso due puntatori e li scambia.
 */
void scambio(pnode &a,pnode &b){
	pnode p=a;
	a=b;
	b=p;
}


/** Applica le proprietà di Heap a un nodo.
 *
 * Partendo dal nodo j controlla ricorsivamente se rispetta
 * le proprietà di Heap. La chiamata ricorsiva avviene sul
 * figlio solo nel caso in cui sia stato effettuato almeno
 * uno scambio (tra padre e figlio).
 */
void MinHeapify(queue &coda, int j, int n){
	int k=j;
	if ((2*j+1<=n) && (coda.elemento[2*j+1]->occorrenze<coda.elemento[k]->occorrenze))
		k=2*j+1;
	if (2*j<=n && coda.elemento[2*j]->occorrenze<coda.elemento[k]->occorrenze)
		k=2*j;
	if (k!=j){
		scambio (coda.elemento[j],coda.elemento[k]);
		MinHeapify(coda, k, n);
		}
}


/** Costruzione di un Min-Heap.
 *
 * La funzione vede l'array come un albero binario e costruisce
 * un Min Heap.
 * In un Min Heap ogni nodo padre è minore dei suoi figli diretti.
 * La funzione comincia a operare a metà dell'array (il primo nodo
 * con figli) fino alla radice e applica le proprietà grazie alla
 * funzione ::MinHeapify.
 */
void BuildMinHeap(queue &coda){
	for (int i=coda.n_elementi/2;i>=1;i--)
		MinHeapify(coda,i,coda.n_elementi);
}


/** Ordinamento decrescente.
 *
 * La funzione permette di ordinare un vettore in modo
 * decrescente utilizzando un Heap. (Heap Sort).
 */
void HeapSort(queue &coda){
	for (int i=coda.n_elementi;i>1;i--){
		scambio(coda.elemento[1],coda.elemento[i]);
		MinHeapify(coda,1,i-1);
		}
}


/** Ristabilisce l'Heap dopo l'inserimento.
 *
 * Dopo un inserimento il nodo da controllare si trova in
 * una foglia e questa funzione controlla se il nodo deve salire
 * nell'Heap. Effettua gli scambi necessari a ristabilire la
 * proprietà di Heap.
 */
void RestoreHeap(queue &coda, int pos){
	while (pos>1 && coda.elemento[pos]->occorrenze<coda.elemento[pos/2]->occorrenze){
		scambio(coda.elemento[pos],coda.elemento[pos/2]);
		pos=pos/2;
		}
}


/** Elimina i caratteri non presenti nel file.
 *
 * La funzione modifica la coda in modo da eliminare gli
 * eleminti che non sono presenti nel file.
 * Restituisce un min Heap.
 */
void pulisci_coda(queue &coda){
	BuildMinHeap(coda);
	HeapSort(coda);
	while(coda.elemento[coda.n_elementi]->occorrenze==0){
		delete[] coda.elemento[coda.n_elementi];
		coda.n_elementi--;
		}
	BuildMinHeap(coda);
}


/** Inserisce un elemento in coda.
 *
 * La funzione prende in ingresso un puntatore a un nodo
 * esistente e lo inserisce nell'ultima posizione della
 * coda. In seguito chiama la ::RestoreHeap che lo fa
 * risalire fino alla posizione corretta nello Heap.
 */
void enqueue(queue &coda, pnode x){
	coda.elemento[++coda.n_elementi]=x;
	RestoreHeap(coda,coda.n_elementi);
}


/** Estrae un elemento dalla coda.
 *
 * Estrae l'elemento in testa (con occorrenza minore)
 * ritornando un puntatore al nodo se la coda non è vuota.
 */
pnode dequeue(queue &coda){
	if (coda.n_elementi<=0) 
		return NULL;
	pnode x= coda.elemento[1];
	scambio(coda.elemento[coda.n_elementi],coda.elemento[1]);
	coda.n_elementi--;
	if (coda.n_elementi>1)
		MinHeapify(coda,1,coda.n_elementi);
	return x;
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
