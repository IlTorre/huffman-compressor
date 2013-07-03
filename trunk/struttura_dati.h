/**
 * @file 
 * File contenente la struttura dati.
 */

#include <iostream>
using namespace std;

#ifdef DEBUG_MODE
const unsigned int MASCHERA_DEBUG = 3;

#define DBG(A, B) {if ((A) & MASCHERA_DEBUG) {B; } }
#else
#define DBG(A, B)
#endif
#define D1(a) DBG(1, a)
#define D2(a) DBG(2, a)


/** Stato di esplorazione.
 *
 * Indica lo stato di esplorazione del ::nodo_t come segue:
 * BIANCO indica che il nodo non è stato esplorato,
 * GRIGIO indica che il nodo è stato scoperto,
 * NERO indica che il nodo è stato scoperto e sono stati scoperti
 * (se esistono) tutti i sottoalberi.
 */
enum colore_t{bianco, grigio, nero};


/** Struttura di un nodo dell'albero.
 *
 * Ciascun nodo dell'albero è composto da:
 * un CARATTERE letto dal file;
 * le relative OCCORRENZE contate scorrendo il file sorgente;
 * un CODICE cioè una stringa dinamica che rappresenta il codice di
 * compressione del carattere;
 * un COLORE (::colore_t) che indica lo stato di esplorazione del nodo;
 * puntatori al PADRE, FIGLIO SINISTRO e DESTRO.
 */
struct nodo_t{
unsigned char carattere;
unsigned long occorrenze;
colore_t colore;
nodo_t *parent;
nodo_t *left;
nodo_t *right;
};


/** Puntatore a un nodo (::nodo_t).
 *
 * Puntatore a un elemento dell'albero, serve per spostarsi gradualmente sui vari nodi.
 */
typedef nodo_t *pnode;


/** Numero massimo di caratteri.
 *
 * La costante è inizializzata a 256+1 che rappresenta il numero di caratteri
 * presenti nella tabella ASCII, più uno spazio vuoto in posizione 0. Questo
 * permette di semplificare le operazioni sullo Heap, le cui posizioni
 * partono da 1.
 */
const int CARATTERI_ASCII = 256+1;


/** Struttura della Coda.
 *
 * Contiene quanti elementi sono nella coda e un vettore con gli elementi.
 */

struct queue{
pnode elemento[CARATTERI_ASCII];
int n_elementi;
};


/**
 */
typedef unsigned char *codice [CARATTERI_ASCII-1];
