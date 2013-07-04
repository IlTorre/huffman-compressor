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
#include <gtk/gtk.h>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include "struttura_dati.h"
#include "comprimi.h"
#include "decomprimi.h"

GtkBuilder *builder;

/** Chiude la finestra aperta.
 *
 * Segnale che chiude la finestra in cui viene premuto il pulsante.
 */
extern "C" void hide_main_window(GtkButton *button, gpointer data){
	gtk_widget_hide(gtk_widget_get_toplevel(GTK_WIDGET(button)));
}


/** Messaggi per l'utente.
 *
 * Questa funzione visualizza una finestra dialogo con l'utente che
 * comunica la riuscita o meno di una certa operazione.
 */
static void mostra_messaggi(bool c1,int flag_cmp){
	GtkWidget *ins_dialog=GTK_WIDGET(gtk_builder_get_object(builder,"messaggi_utente"));
	GtkLabel *label=GTK_LABEL(gtk_builder_get_object(builder,"messaggio"));
	GtkImage *image=GTK_IMAGE(gtk_builder_get_object(builder,"img_result"));
	if(flag_cmp)
	{
		if(c1)
		{
			gtk_label_set_text(label,"Compressione del file riuscita");
			gtk_image_set_from_stock(image,"gtk-yes",  GTK_ICON_SIZE_BUTTON);
		}
		else
		{
			gtk_label_set_text(label,"Compressione del file non riuscita");
			gtk_image_set_from_stock(image,"gtk-dialog-warning",  GTK_ICON_SIZE_BUTTON);
		}
	}
	else
	{
		if(c1)
		{
			gtk_label_set_text(label,"Decompressione del file riuscita");
			gtk_image_set_from_stock(image,"gtk-yes",  GTK_ICON_SIZE_BUTTON);
		}
		else
		{
			gtk_label_set_text(label,"Decompressione del file non riuscita");
			gtk_image_set_from_stock(image,"gtk-dialog-warning",  GTK_ICON_SIZE_BUTTON);
		}
	}
	
	gtk_widget_show_all(ins_dialog);
	
}
/** segnale che gestisce l'evento click del pulsante comprimi 
* apre la finestra di dialogo adibita al salvataggio
*/
extern "C" void handler_comprimi(GtkButton *button,gpointer data)
{	
	gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(builder,"mex_esegui")),"Comprimi");
	gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(builder,"msg")),"scegli dove salvare e il nome del file compresso");
	
	gchar *sorg=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(data));
	if(sorg==NULL)
	{		
		mostra_messaggi(false,1);
		return;
	}
	GtkWidget *salvataggio=GTK_WIDGET(gtk_builder_get_object(builder,"salva"));
	gtk_widget_show_all(salvataggio);
}
/** chiude la finestra adibita al salvataggio */
static void chiudi_salvataggio()
{
	GtkWidget *salvataggio=GTK_WIDGET(gtk_builder_get_object(builder,"salva"));
	gtk_widget_hide(salvataggio);
}
/** crea il percorso assouluto al file creato agganciando a una stessa stringa
* il percorso del file, il suo nome, e l'estensione adeguata all'operazione in 
* corso
*/
static char * crea_destinazione(gpointer data,bool flag)
{
	char *percorso=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(data));
	if(percorso==NULL)
		return NULL;
	//cout<<"percorso: "<<percorso<<endl;
	
	GtkEntry *entry=GTK_ENTRY(gtk_builder_get_object(builder,"nome_file"));
	const char *testo_file=gtk_entry_get_text(entry);
	//cout<<"nome file: "<<nome_file<<endl;
	istringstream s(testo_file);
	char *nome_file=new char [strlen(testo_file)];
	s>>nome_file;
	//cout<<nome_file<<endl;
	if((nome_file[0]<'A' || nome_file[0]>'z'))
	{		
		return NULL;
	}
	char *dest=new char[strlen(percorso)+strlen(nome_file)+4];
	strcpy(dest,percorso);
	strcat(dest,"/");
	strcat(dest,nome_file);
	if(flag)
		strcat(dest,".huf");
	else
		strcat(dest,".txt");
		
	return dest;
}
/** funzione che effettua la compressione del .txt selezionato 
* sfrutta la funzione ::crea_destinazione che crea il percorso assoluto
* per trovare dove salvare il file compresso con nome scelto dall'utente.
* Utilizza la funzione ::comprimi per effettuare tutte le operazioni
* relative alla compressione del file .txt
*/
static void effettua_compressione(gpointer data)
{
	gchar *sorg=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(gtk_builder_get_object(builder,"chooser_compressore")));
	cout<<"sorgente"<<sorg<<endl;
	
	char *dest=crea_destinazione(data,1);
	
	if(dest==NULL)
	{
		chiudi_salvataggio();
		mostra_messaggi(false,1);
		return;
	}
	//cout<<"dest: "<<dest<<endl;
	
	D1(cout<<"Chiamo comprimi, gli passo sorgente = "<<sorg<<" e destinazione = "<<dest<<endl);
	
	bool c1=comprimi(sorg,dest);
	
	chiudi_salvataggio();
	
	mostra_messaggi(c1,1);
}
/** segnale che gestisce l'evento click del pulsante decomprimi
* questo segnale apre un finestra di dialogo dove poter salvare 
* e dare un nome al file che si crea con le decompressione
* controlla inoltre che l'utente abbia selezionato un file .huf 
* per da decomprimere
*/
extern "C" void handler_decomprimi(GtkButton *button,gpointer data)
{
	gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(builder,"mex_esegui")),"Decomprimi");
	gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(builder,"msg")),"scegli dove salvare e il nome del file decompresso");
	
	gchar *sorg=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(data));
	if(sorg==NULL)
	{		
		mostra_messaggi(false,0);
		return;
	}
	GtkWidget *salvataggio=GTK_WIDGET(gtk_builder_get_object(builder,"salva"));
	gtk_widget_show_all(salvataggio);
}
/** funzione che effettua la decompressione del .huf selezionato 
* sfrutta la funzione ::crea_destinazione che crea il percorso assoluto
* per trovare dove salvare il file decompresso con nome scelto dall'utente.
* Utilizza la funzione ::decomprimi per effettuare tutte le operazioni
* relative alla decompressione del file .huf
*/
static void effettua_decompressione(gpointer data)
{
	gchar *sorg=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(gtk_builder_get_object(builder,"chooser_decompressore")));
	cout<<"sorgente"<<sorg<<endl;
	
	char *dest=crea_destinazione(data,0);
	
	if(dest==NULL)
	{
		chiudi_salvataggio();
		mostra_messaggi(false,0);
		return;
	}
	//cout<<"dest: "<<dest<<endl;
	
	D1(cout<<"Chiamo comprimi, gli passo sorgente = "<<sorg<<" e destinazione = "<<dest<<endl);
		
	bool c2=decomprimi(sorg,dest);
	
	chiudi_salvataggio();
	
	mostra_messaggi(c2,0);
}
/** segnale paragonbile a un front-end permette di decidere quale operazione eseguire 
* se la compressione o la decompressione del file
*/
extern "C" void handler_scegli_op(GtkButton *button,gpointer data)
{
	GtkLabel *label=GTK_LABEL(gtk_builder_get_object(builder,"mex_esegui"));
	const char *testo=gtk_label_get_text(label);
	if(strcmp(testo,"Comprimi")==0)
		effettua_compressione(data);
	else
		effettua_decompressione(data);
}
/** link alla documentazione */
extern "C" void handler_carica_documentazione(GtkMenuItem *item,gpointer data)
{
	system("firefox ../doc/html/index.html");
}
int main(int argc,char *argv[]){


	/**inizializzazione gtk */
	gtk_init(&argc,&argv);
	
	builder=gtk_builder_new();
	
	/** aggiunga interfaccia grafica grafica.glade */
	gtk_builder_add_from_file(builder,"grafica.glade",NULL);
	
	/** aggiunta dei filtri per la selzione dei file ai filechooser, i filtri sono stati creati tramite glade
	* qui avviene solo il settaggio del relativo nome e l'aggangio dei filtri ai filechooser
	*/
	GtkFileFilter *filtro=GTK_FILE_FILTER(gtk_builder_get_object(builder,"filtro_file"));
	/** primo filtri riferito al chooser del compressore rende selezionabili solo i file 
	* con estensione .txt 
	*/
	gtk_file_filter_set_name(filtro,".txt");
	
	GtkFileFilter *filtro_dec=GTK_FILE_FILTER(gtk_builder_get_object(builder,"filtro_file2"));
	/** secondo filtro riferito al chooser del decompressore rende selezionabili solo i file 
	* con estensione .huf
	*/
	gtk_file_filter_set_name(filtro_dec,"*.huf");
	
	/** aggancio primo filtro */
	GtkFileChooser *chooser=GTK_FILE_CHOOSER(gtk_builder_get_object(builder,"chooser_compressore"));
	gtk_file_chooser_add_filter(chooser,filtro);
	
	/** agggancio secondo filtro */
	GtkFileChooser *chooser_dec=GTK_FILE_CHOOSER(gtk_builder_get_object(builder,"chooser_decompressore"));
	gtk_file_chooser_add_filter(chooser_dec,filtro_dec);
	
	
	/** conessione dei segnali con l'interfaccia */
	gtk_builder_connect_signals(builder,NULL);
	
	/** loop principale */
	gtk_main();

	return 0;
	//return (static_cast<int>(c1 && c2));
}
