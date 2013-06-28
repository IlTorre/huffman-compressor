#include <iostream>
#include <fstream>
#include <cstring>
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
		coda.elemento[i]->carattere=static_cast<unsigned char>(i-1);
		}
	coda.n_elementi=0;
}

/** Conta le occorrenze di ogni lettera.
 *
 * Prende in ingresso il file sorgente e conta le occorrenze di tutte le
 * lettere. Riserva inoltre un carattere per segnalare la fine del file.
 * La funzione restituisce un valore booleano che segnala se
 * l'operazione è andata a buon fine.
 */
bool conta_occorrenze(char sorgente[], queue &coda){
	unsigned char car;
	ifstream f (sorgente);
	if (!f) 
		return false;
	while(f.read(reinterpret_cast<char *>(&car), sizeof (car)))
		coda.elemento[static_cast<int>(car)+1]->occorrenze++;
	coda.n_elementi=CARATTERI_ASCII-1;
	f.close();
	coda.elemento[1]->occorrenze++;
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
 * Restituisce un min Heap e il numero di caratteri presenti.
 */
int pulisci_coda(queue &coda){
	BuildMinHeap(coda);
	HeapSort(coda);
	while(coda.elemento[coda.n_elementi]->occorrenze==0){
		delete[] coda.elemento[coda.n_elementi];
		coda.n_elementi--;
		}
	BuildMinHeap(coda);
	return coda.n_elementi;
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
BuildMinHeap(coda);
}
 

/** Aggancia padre e figli.
 *
 * La funzione riceve in ingresso tre puntatori. Il primo prende
 * come figli gli altri due. La funzione si limita all'aggiornamento
 * dei puntatori degli elementi passati.
 */
void aggancia (pnode &p, pnode &l, pnode &r){
	l->parent = p;
	r->parent = p;
	p->left = l;
	p->right= r;
}
/** Unisce due nodi.
 *
 * La funzione crea un nuovo nodo che ha come carattere un valore
 * non significativo e come occorrenze la somma delle occorrenze
 * dei nodi passati. Li ::aggancia come figli e infine ritorna un
 * puntatore all'oggetto creato.
 */
pnode unisci_nodi(pnode &x, pnode &y){
	pnode p = new nodo_t;
	p->carattere=-1;
	p->occorrenze = x->occorrenze + y->occorrenze;
	p->parent=NULL;
	aggancia(p,x,y);
	return p;
}


/** Crea l'albero di compressione.
 *
 * La funzione estrae dalla coda (::dequeue) due elementi e li
 * unisce (::unisci_nodi), rimette in coda (::enqueue) il nuovo
 * nodo ottenuto. Quando in coda rimane un solo elemento
 * ritorna il puntatore alla radice dell'albero creato e svuota
 * la coda.
 */
pnode crea_albero(queue &coda){
	while (coda.n_elementi>1){
		pnode x = dequeue (coda);
		pnode y = dequeue (coda);
		pnode p = unisci_nodi(x,y);
		enqueue (coda,p);
		}
	pnode root = coda.elemento[1];
	coda.n_elementi = 0;
	return root;
}


/** Resetta i colori dei nodi.
 *
 * La funzione scorre tutti i nodi dell'albero e inizilializza
 * a bianco tutti i nodi interni, solo le foglie a nero.
 */
void resetta_colori(const pnode &root){
	pnode p = root;
	if (p->left == NULL && p->right == NULL){
		p->colore = nero;
		return;
		}
	p->colore = bianco;
	if (p->left != NULL)
		resetta_colori(p->left);
	if (p->right != NULL)
		resetta_colori(p->right);	
}


/** Sistema i colori dei nodi
*
* Aggiusta i colori dei genitori dei nodi appena esplorati in
* modo da saltare la stampa di nodi privi di chiave
* significativa.
* Nodo BIANCO indica un nodo inesplorato.
* Nodo GRIGIO indica esplorato il sottoalbero di sinistra.
* Nodo NERO indica che il nodo è stato esplorato completamente
* (sia in suo sottoalbero di sinistra che quello di destra è
* stato scoperto).
*
*/
void controllo_colore (pnode x, const pnode root){
	if (x->colore == bianco)
		x->colore = grigio;
	else{
		x->colore = nero;
		if (x != root)
			controllo_colore (x->parent, root);
		}
}


/** Assegna i codici ai caratteri.
 *
 * Quando esplorando l'albero incontriamo una foglia questa
 * funzione viene chiamata e assegna il codice trovato fino a
 * quel momento alla posizione del carattere trovato nella foglia.
 */
void alloca(codice &conversione, unsigned char car, char buf[]){
	int lun = strlen(buf)+1;
	conversione[static_cast<int>(car)] = new char [lun];
	strcpy(conversione[static_cast<int>(car)],buf);
}
 
 
/** Genera una matrice contenente i codici di conversione.
 *
 * Esplora l'albero e tramite ::alloca assegna man mano tutti i
 * codici ai rispettivi caratteri.
 */
void genera_codice(const pnode &root, codice &conversione, const int num_caratteri){
	char *buffer = new char [num_caratteri];
	resetta_colori(root);
	pnode x = root;
	int i = 0;
	while(root->colore != nero){
		if (x->colore == bianco){
			buffer[i++]='0';
			x=x->left;
			}
		if (x->colore == grigio){
			buffer[i++]='1';
			x=x->right;
			}
		if (x->colore == nero){
			buffer[i] = '\0';
			alloca(conversione, x->carattere, buffer);
			controllo_colore(x->parent, root);
			x = root;
			i=0;
			}
		}
}


/** Funzione ricorsiva per la creazione del preambolo.
 *
 * La funzione si occupa di esplorare l'albero e di salvare
 * le lettere presenti nelle foglie. Salva il risltato in un
 * buffer.
 */
void preambolo_ric(unsigned char preambolo[], pnode &x, int &i){
	 if (x->left == NULL && x->right == NULL){
 		preambolo[i++] = x->carattere;
 		return;
 		}
 	if (x->left != NULL)
		preambolo_ric(preambolo,x->left,i);
 	if (x->right != NULL)
		preambolo_ric(preambolo,x->right,i);
 	
}
/** Crea il preambolo.
 *
 * La funzione crea il preambolo necessario per la decompressione
 * futura del file. Il preambolo è composto da un byte che indica
 * quante sono le lettere pressenti nel file più tutte le lettere
 * in ordine da sinistra verso destra nell'albero.
 * Questa configurazione permette al decompressore di ricostruire
 * l'albero biario e di conseguenza il codice di decompressione.
 */
void crea_preambolo(unsigned char preambolo[], pnode root, int n_caratteri){
	preambolo[0] = static_cast<unsigned char>(n_caratteri);
	int i = 1;
	pnode x = root;
	preambolo_ric(preambolo,x,i);
}


/** Crea il codice identificativo dell'albero.
 *
 * Esplora l'albero in ordine e assegna FALSE per ogni ramo sinistro
 * che incontra e TRUE per ogni ramo destro. Ottiene un codice
 * univoco e identificativo della struttura dell'albero.
 */
void codice_albero(bool albero[], pnode p, int &i){
	if (p != NULL){
		if (p->left != NULL){
			albero[i++] = false;
			codice_albero(albero, p->left, i);
			}
		if (p->right != NULL){
			albero[i++] = true;
			codice_albero(albero, p->right, i);
			}
		}
}


/** Setta l'ultimo bit.
 *
 * La funzione prende in ingresso una maschera e ne setta l'ultimo bit.
 */
void setlastbit(char &MASK){
MASK = MASK | 1;
}


/** Scrive il preambolo sul file.
 *
 * La funzione riceve in insgresso lo stream sul quale scrive il
 * preambolo.
 */
void scrivi_preambolo(ostream &f2, pnode root, int n_caratteri){
	unsigned char preambolo[n_caratteri];
	crea_preambolo(preambolo,root,n_caratteri);
	for(int j=0;j<=n_caratteri;j++)
		f2<<preambolo[j];
}


/** Scrive il codice identificativo dell'albero sul file.
 *
 * Utilizzando la funzione ::codice_albero scrive il codice
 * ottenuto nel file. Se rimangono bit sul buffer verranno
 * completati in seguito.
 */
void scrivi_albero (ostream &f2, pnode root, int n_caratteri, char &BUFFER, int &l_buffer){
	bool albero[2*(n_caratteri-1)];
	int i=0;
	codice_albero(albero, root, i);
	for(int k=0;k<2*(n_caratteri-1);k++){
		BUFFER = BUFFER<<1;
		if (albero [k])
			setlastbit(BUFFER);
		l_buffer++;
		if (l_buffer == 8){
			f2<<BUFFER;
			BUFFER = 0;
			l_buffer = 0;
			}
	}
}


/** Svuota il buffer.
 *
 * La funzione controlla se ci sono bit significativi nel
 * ::BUFFER e se presenti li scrive nel file.
 */
void svuota_buffer(ostream &f2, char &BUFFER, int &l_buffer){
	if (l_buffer > 0){
		BUFFER = BUFFER<<(8-l_buffer);
		f2<<BUFFER;
		l_buffer = 0;
		BUFFER = 0;
		}
}


/** Scrive il codice di un dato carattere.
 *
 * La funzione riceve in input un caratere e scrive sul
 * flusso il codice relativo.
 */
void scrivi_codice(ostream &f2, codice conversione, unsigned char car, char &BUFFER, int &l_buffer){
	int i=0;
	while(conversione[static_cast<int>(car)][i] != '\0'){
		BUFFER = BUFFER<<1;
		if (conversione[static_cast<int>(car)][i++] == '1')
			setlastbit(BUFFER);
		l_buffer++;
		if (l_buffer == 8){
			f2<<BUFFER;
			l_buffer = 0;
			BUFFER = 0;
			}
		}
}



/** Converte l'intero documento.
 *
 * La funzione scorre il file sorgente e grazie a
 * ::scrivi_codice, carattere per carattere comprime
 * il file.
 */
bool converti(ostream &f2, codice conversione, const char sorgente[], char &BUFFER, int &l_buffer){
	ifstream f3 (sorgente);
	if(!f3)
		return false;
	unsigned char car;
	while(f3.read(reinterpret_cast<char *>(&car), sizeof (car)))
		scrivi_codice (f2, conversione, car, BUFFER,l_buffer);
	f3.close();
	return true;

}


/** Inserisce il terminatore.
 * Per segnalare la fine del documento è stato riservato
 * un codice di terminazione. Esso si riferisce al carattere
 * speciale della tabella ASCII di posizione 0 non utilizzato
 * nei file di testo. Servirà in fase di decompressione per
 * segnalare la fine del file.
 */
void inserisci_terminatore(ostream &f2, codice conversione,char &BUFFER, int &l_buffer){
	scrivi_codice(f2,conversione,0,BUFFER, l_buffer);	
}


/** Compone il file compresso.
 *
 *La funzione si avvale di altre funzioni per creare tutte
 * le parti necessarie al file compresso. Principalmente si
 * occupa dell'apertura del flusso di scrittura su file che
 * passa alle altre funzioni che creano pezzo per pezzo il
 * file.
 */
bool scrivi_file(const char sorgente[], const char destinazione[], int n_caratteri, pnode root,codice conversione){
	ofstream f2 (destinazione);
	if (!f2)
		return false;
	
	scrivi_preambolo(f2, root, n_caratteri);
	char BUFFER = 0;
	int l_buffer = 0;
	scrivi_albero(f2, root, n_caratteri, BUFFER, l_buffer);
	bool cont1 = converti(f2, conversione, sorgente, BUFFER, l_buffer);
	inserisci_terminatore (f2, conversione, BUFFER, l_buffer);
	svuota_buffer(f2, BUFFER, l_buffer);
	f2.close();
	return (cont1);

}


//DEBUG
void DDD(const pnode p, bool foglie){
	if (p != NULL){
		DDD(p->left, foglie);
		if (foglie && p->carattere != -1)
			cout<<p->colore<<endl;
		if (!foglie && p->carattere == -1)
			cout<<p->colore<<endl;
		DDD(p->right, foglie);
		}
}

bool comprimi (char sorgente[], char destinazione[]){
	queue coda;
	inizializza_coda(coda);
	conta_occorrenze(sorgente, coda);
	int num_caratteri = pulisci_coda(coda);
	pnode root = crea_albero(coda); //mettere const?
	codice conversione;
	genera_codice(root,conversione,num_caratteri);
	return(scrivi_file(sorgente, destinazione, num_caratteri, root, conversione));
}
