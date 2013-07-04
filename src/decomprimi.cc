#include <iostream>
#include <fstream>
#include <cstring>
#include "struttura_dati.h"


/** Ricava il numero dei caratteri.
 *
 * La funzione legge dal file sorgente il primo byte e lo associa
 * al numero di caratteri diversi presenti nel file originale.
 */
int numero_caratteri(istream &f4){
	D1(cout<<"sono entrato in numero_caratteri"<<endl);
	unsigned char car;
	f4.read(reinterpret_cast<char *>(&car), sizeof (car));
	return (static_cast<int>(car));
}


/** Ricava i caratteri.
 *
 * La funzione legge dal file sorgente i caratteri presenti nel file originale.
 */
unsigned char *ricava_caratteri (istream &f4, int n_caratteri){
	D1(cout<<"sono entrato in ricava_caratteri, n_caratteri = "<<n_caratteri<<endl);
	unsigned char *car = new unsigned char [n_caratteri];
	for (int i=0;i<n_caratteri;i++)
		f4.read(reinterpret_cast<char *>(&car[i]), sizeof (car[i]));
	return car;
}


/** Controlla se è settato il bit.
 * La funzione controlla lo stato del bit più significativo.
 * Ritorna un valore booleano.
 */
bool checkbit(unsigned char BUFFER){
	unsigned char MASK = 128;
	return (BUFFER & MASK);
}


/** Ricava il codice binario univoco dell'albero.
 *
 * Legge dal file il codice binario che identifica l'albero.
 */
bool * leggi_albero (istream &f4, unsigned char &BUFFER, int &l_buffer, int l_albero){
	D1(cout<<"sono entrato in leggi_albero, BUFFER = "<<BUFFER<<" l_buffer = "<<
	l_buffer<<" l_albero = "<<l_albero<<endl);
	bool *albero = new bool [l_albero];
	for (int i = 0; i<l_albero; i++){
		if (l_buffer == 0){
			f4.read(reinterpret_cast<char *>(&BUFFER), sizeof (BUFFER));
			l_buffer = 8;
			}
		albero[i] = checkbit(BUFFER);
		BUFFER = BUFFER<<1;
		l_buffer--;
		}
	return albero;
}


/** Crea un nodo non inizializzato.
 *
 * La funzione alloca un nodo e inizializza a NULL i puntatori.
 * Ritorna un puntatore al nuovo elemento creato.
 */
pnode crea_nodo (){
	D2(cout<<"creo un nuovo nodo"<<endl);
	pnode x = new nodo_t;
	x->right = NULL;
	x-> left = NULL;
	x->parent = NULL;
	return x;
}


/** Costruisce l'albero di decompressione.
 *
 * La funzione scorre il codice identificativo dell'albero generato
 * da ::leggi_albero e lo ricostruisce.
 * La funzione ritorna un puntatore alla radice.
 */
pnode costruisci_albero(const unsigned char caratteri[],const bool albero[],int l_albero){
	D1(cout<<"sono entrato in costruisci_albero, caratteri = "<<caratteri<<" l_albero = "<<l_albero<<endl);
	pnode root = crea_nodo();
	pnode x = root;
	int conta_car = 0;
	for (int i = 0; i<l_albero; i++){
		D2(cout<<"albero ["<<i<<"] = "<<albero[i]<<endl);
		pnode temp = crea_nodo();
		if (!albero[i]){
			D2(cout<<"lo inserisco come figlio sinistro"<<endl);
			temp->parent = x;
			x->left = temp;
			x = temp;
			}
		else
			if (x->left == NULL){
				D2(cout<<"scrivo la lettera nella foglia e risalgo finchè non trovo un ramo destro libero"<<endl);
				x->carattere = caratteri[conta_car++];
				x=x->parent;
				while (x->right != NULL)
					x = x->parent;
				D2(cout<<"lo inserisco come figlio destro"<<endl);
				temp->parent = x;
				x->right = temp;
				x = temp;
				}
			else{
				D2(cout<<"lo inserisco come figlio destro"<<endl);
				temp->parent = x;
				x->right = temp;
				x=temp;
				}
		}
	x->carattere = caratteri[conta_car];
	return root;
}

/** Crea l'albero di decompressione.
 * Fornisce tutti gli elementi necessari per la creazione 
 * dell'albero alla funzione ::costruisci_albero.
 * Ritorna un puntatore alla radice dell'albero
 */
pnode crea_albero (istream &f4, unsigned char &BUFFER, int &l_buffer){
	D1(cout<<"sono entrato in crea_albero, BUFFER = "<<BUFFER<<" l_buffer = "<<l_buffer<<endl);
	int n_caratteri = numero_caratteri(f4);
	unsigned char *caratteri = ricava_caratteri(f4, n_caratteri);
	int l_albero=2*(n_caratteri-1);
	bool *albero = leggi_albero (f4, BUFFER, l_buffer, l_albero);
	pnode root = costruisci_albero(caratteri,albero,l_albero);
	delete [] caratteri;
	delete [] albero;
	return root;
}


/** Decodifica carattere per carattere.
 *
 * La funzione scorre i bit del file compresso e li trasforma
 * grazie all'albero costruito precedentemente in caratteri che
 * scrive sul file decompresso.
 */
void decodifica(istream &f4,ostream &f5, unsigned char &BUFFER, int &l_buffer, pnode root){
	D1(cout<<"sono entrato in scrivi_albero, BUFFER = "<<BUFFER<<" l_buffer = "<<l_buffer<<endl);
	pnode x = root;
	while (true){
		if (l_buffer == 0){
			D2(cout<<"il buffer è vuoto, leggo un carattere dal file compresso"<<endl);
			f4.read(reinterpret_cast<char *>(&BUFFER), sizeof (BUFFER));
			l_buffer = 8;
			}
		if (x->left == NULL && x->right == NULL){
			D2(cout<<"ho trovato una foglia, quindi un carattere"<<endl);
			if (x->carattere == 0){
				D2(cout<<"è il terminatore, fine"<<endl);
				return;
				}
			D2(cout<<"scrivo sul file decompresso il carattere e riparto dalla radice"<<endl);
			f5<<x->carattere;
			x = root;
			}	
		if(!checkbit(BUFFER)){
			D2(cout<<"scendo sul figlio sinistro"<<endl);
			x=x->left;
			}
		else{
			D2(cout<<"scendo sul figlio destro"<<endl);
			x=x->right;
			}
		BUFFER = BUFFER<<1;	
		l_buffer--;
		D2(cout<<"ho shiftato il buffer, ora vale "<<static_cast <int>(BUFFER)<<
		" e l_buffer vale "<<l_buffer<<endl);
		}	
}


bool decomprimi(char sorgente[], char destinazione[]){
	D1(cout<<"sono entrato in decomprimi, sorgente = "<<sorgente<<" destinazione = "<<destinazione<<endl);
	ifstream f4(sorgente);
	if (!f4)
		return false;

	unsigned char BUFFER = 0;
	int l_buffer = 0;
	pnode root = crea_albero(f4,BUFFER,l_buffer);
	ofstream f5 (destinazione);
	if (!f5)
		return false;
	decodifica (f4, f5, BUFFER, l_buffer, root);
	f4.close();
	f5.close();
	return true;
}
