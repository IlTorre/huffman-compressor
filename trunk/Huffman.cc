/** Programma principale.
 * @mainpage Programma di compressione e decompressione file.
 *
 * Programma per la compressione e la decompressione dei File mediante
 * l'algoritmo di Huffman.
 *
 *
 * Nella fase di COMPRESSIONE il programma legge un file di testo dato in
 * input e conta le occorrenze di ogni carattere e assegnando ai caratteri
 * più frequenti codici più brevi.
 * Al termine restituisce un file binario teoricamente compresso.
 *
 * Nella fase di DECOMPRESSIONE il programma riceve in input il file binario
 * compresso e mediante opportune operazione ricostruisce il codice di
 * compressione e ricostruisce il file originario.
 * Al termine restituisce il file originario in formato di testo.
 * 
 * @author Marco Torreggiani e Alberto Vezzani
 */


#include <iostream>
//#include <gtk/gtk.h>
#include "struttura_dati.h"
#include "comprimi.h"
#include "decomprimi.h"


int main(){
	char sorg[]="/home/studente/Documenti/commedia.txt";
	char dest[]="/home/studente/Documenti/compresso.huf";
	D1(cout<<"Chiamo comprimi, gli passo sorgente = "<<sorg<<" e destinazione = "<<dest<<endl);
	bool c1 = comprimi(sorg,dest);
	char dec[]="/home/studente/Documenti/decompresso.txt";
	D1(cout<<"Chiamo decomprimi, gli passo sorgente = "<<dest<<" e destinazione = "<<dec<<endl);
	bool c2 = decomprimi (dest,dec);
	return (static_cast<int>(c1 && c2));
}
