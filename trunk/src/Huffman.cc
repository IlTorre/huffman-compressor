/**
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

/** Controlla che l'utente abbia selezionato un file.
 *
 * Per evitare che l'utente possa comprimere cartelle,
 * questa funzione controlla l'estensione del file selezionato.
 */
static void controlla_selezione(char ctrl[],gchar *sorg){
	int i=0;
	int j=(strlen(sorg)-4);
	int lungh=strlen(sorg);
	for(;i<5 && j<lungh;i++,j++)
		ctrl[i]=sorg[j];
	ctrl[4]='\0';
}


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
 * Il parametro c1 indica la riuscita o no dell'operazione mentre
 * il flag_cmp indica se l'operazione è una compressione o una
 * decompressione.
 */
static void mostra_messaggi(bool c1,int flag_cmp){
	GtkWidget *ins_dialog=GTK_WIDGET(gtk_builder_get_object(builder,"messaggi_utente"));
	GtkLabel *label=GTK_LABEL(gtk_builder_get_object(builder,"messaggio"));
	GtkImage *image=GTK_IMAGE(gtk_builder_get_object(builder,"img_result"));
	if(flag_cmp){
		if(c1){
			gtk_label_set_text(label,"Compressione del file riuscita");
			gtk_image_set_from_stock(image,"gtk-yes",  GTK_ICON_SIZE_BUTTON);
			}
		else{
			gtk_label_set_text(label,"Compressione del file non riuscita");
			gtk_image_set_from_stock(image,"gtk-dialog-warning",  GTK_ICON_SIZE_BUTTON);
			}
		}
	else{
		if(c1){
			gtk_label_set_text(label,"Decompressione del file riuscita");
			gtk_image_set_from_stock(image,"gtk-yes",  GTK_ICON_SIZE_BUTTON);
			}
		else{
			gtk_label_set_text(label,"Decompressione del file non riuscita");
			gtk_image_set_from_stock(image,"gtk-dialog-warning",  GTK_ICON_SIZE_BUTTON);
			}
		}
	gtk_widget_show_all(ins_dialog);	
}


/** Segnale del bottone compressore.
 *
 * Segnale che gestisce l'evento click del pulsante compressore: 
 * apre la finestra di dialogo adibita al salvataggio del file compresso,
 * ricava il percorso di salvataggio e comunica lo stato dell'operazione.
 */
extern "C" void handler_comprimi(GtkButton *button,gpointer data){	
	gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(builder,"mex_esegui")),"Comprimi");
	gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(builder,"msg")),"Scegli dove salvare e il nome del file compresso");
	gchar *sorg=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(data));
	char *ctrl=new char[5];
	if (sorg != NULL)
		controlla_selezione(ctrl,sorg);
	if(sorg==NULL || strcmp(ctrl,".txt")!=0){		
		mostra_messaggi(false,1);
		return;
		}
	GtkWidget *salvataggio=GTK_WIDGET(gtk_builder_get_object(builder,"salva"));
	gtk_widget_show_all(salvataggio);
}


/** Chiude la finestra adibita al salvataggio.
 *
 * Nasconde la finestra di salvataggio.
 */
static void chiudi_salvataggio(){
	GtkWidget *salvataggio=GTK_WIDGET(gtk_builder_get_object(builder,"salva"));
	gtk_widget_hide(salvataggio);
}


/** Ottiene il percorso completo di salvataggio.
 *
 * Ricava per prima cosa il percorso selezionato dall'utente nel file_chooser,
 * in seguito ricava il nome che l'utente vuole assegnare al file elaborato,
 * infine crea una stringa e vi inserisce il percorso, aggiunge uno /,
 * aggiunge il nome file poi l'estensione in base al flag che segnala se è
 * in fase di compressione o di decompressione.
 * Es: PERCORSO/NOMEFILE.ESTENSIONE
 */
static char * crea_destinazione(gpointer data,bool flag){
	char *percorso=gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER(data));
	if(percorso==NULL)
		return NULL;
	GtkEntry *entry=GTK_ENTRY(gtk_builder_get_object(builder,"nome_file"));
	const char *testo_file=gtk_entry_get_text(entry);
	istringstream s(testo_file);
	char *nome_file=new char [strlen(testo_file)];
	s>>nome_file;
	if(nome_file[0]<'A' || nome_file[0]>'z')		
		return NULL;
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


/** Effettua la compressione del file.
 * 
 * La funzione effettua la compressione del .txt selezionato 
 * sfrutta la funzione ::crea_destinazione che crea il percorso assoluto
 * in cui salvare il file compresso con il nome scelto dall'utente.
 * Utilizza la funzione ::comprimi per effettuare tutte le operazioni
 * relative alla compressione del file.
 */
static void effettua_compressione(gpointer data){
	gchar *sorg=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(gtk_builder_get_object(builder,"chooser_compressore")));	
	char *dest=crea_destinazione(data,1);
	if(dest==NULL){
		chiudi_salvataggio();
		mostra_messaggi(false,1);
		return;
		}	
	bool c1=comprimi(sorg,dest);
	chiudi_salvataggio();
	mostra_messaggi(c1,1);
}


/** Segnale del bottone decompressore.
 *
 * Segnale che gestisce l'evento click del pulsante decompressore: 
 * apre la finestra di dialogo adibita al salvataggio del file decompresso,
 * ricava il percorso di salvataggio e comunica lo stato dell'operazione.
 */
extern "C" void handler_decomprimi(GtkButton *button,gpointer data){
	gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(builder,"mex_esegui")),"Decomprimi");
	gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(builder,"msg")),"Scegli dove salvare e il nome del file decompresso");
	gchar *sorg=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(data));
	char *ctrl=new char[5];
	if (sorg != NULL)
		controlla_selezione(ctrl,sorg);
	if(sorg==NULL || strcmp(ctrl,".huf")!=0){		
		mostra_messaggi(false,0);
		return;
		}
	GtkWidget *salvataggio=GTK_WIDGET(gtk_builder_get_object(builder,"salva"));
	gtk_widget_show_all(salvataggio);
}


/** Effettua la decompressione del file.
 * 
 * La funzione effettua la decompressione del .huf selezionato 
 * sfrutta la funzione ::crea_destinazione che crea il percorso assoluto
 * in cui salvare il file compresso con il nome scelto dall'utente.
 * Utilizza la funzione ::comprimi per effettuare tutte le operazioni
 * relative alla decompressione del file.
 */
static void effettua_decompressione(gpointer data){
	gchar *sorg=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(gtk_builder_get_object(builder,"chooser_decompressore")));
	char *dest=crea_destinazione(data,0);
	if(dest==NULL){
		chiudi_salvataggio();
		mostra_messaggi(false,0);
		return;
		}	
	bool c2=decomprimi(sorg,dest);
	chiudi_salvataggio();
	mostra_messaggi(c2,0);
}


/** Sceglie l'operazione da eseguire.
 *
 * In base all'operazione scelta richiama la funzione corretta
 * (::effettua_compressione o ::effettua_decompressione). Per
 * sapere quale eseguire controlla lo stato della label del
 * bottone nella finestra di salvataggio.
 */
extern "C" void handler_scegli_op(GtkButton *button,gpointer data){
	GtkLabel *label=GTK_LABEL(gtk_builder_get_object(builder,"mex_esegui"));
	const char *testo=gtk_label_get_text(label);
	if(strcmp(testo,"Comprimi")==0)
		effettua_compressione(data);
	else
		effettua_decompressione(data);
}


/** Link alla documentazione
 *
 * Permette di aprire una pagina di Firefox con la relativa documentazione
 * in formato HTML.
 */
extern "C" void handler_carica_documentazione(GtkMenuItem *item,gpointer data){
	system("firefox ../doc/html/index.html");
}


int main(int argc,char *argv[]){


	/* Inizializzazione gtk. */
	gtk_init(&argc,&argv);
	builder=gtk_builder_new();
	
	
	/* Aggiunge l'interfaccia grafica creata con Glade */
	gtk_builder_add_from_file(builder,"grafica.glade",NULL);
	
	
	/* Creazione primo filtro.
	 * 
	 * Ricava un puntatore al filtro creato con Glade e lo imposta per accettare i .txt .
	 * Ricava un puntatore al file_chooser e in seguito aggiunge il filtro al file_chooser.
	 */
	GtkFileFilter *filtro=GTK_FILE_FILTER(gtk_builder_get_object(builder,"filtro_file"));
	gtk_file_filter_set_name(filtro,".txt");
	GtkFileChooser *chooser=GTK_FILE_CHOOSER(gtk_builder_get_object(builder,"chooser_compressore"));
	gtk_file_chooser_add_filter(chooser,filtro);
	
	
	/* Creazione secondo filtro.
	 * 
	 * Ricava un puntatore al filtro creato con Glade e lo imposta per accettare i .huf .
	 * Ricava un puntatore al file_chooser e in seguito aggiunge il filtro al file_chooser.
	 */
	GtkFileFilter *filtro_dec=GTK_FILE_FILTER(gtk_builder_get_object(builder,"filtro_file2"));
	gtk_file_filter_set_name(filtro_dec,"*.huf");
	GtkFileChooser *chooser_dec=GTK_FILE_CHOOSER(gtk_builder_get_object(builder,"chooser_decompressore"));
	gtk_file_chooser_add_filter(chooser_dec,filtro_dec);
	
	
	/* Connessione dei segnali con l'interfaccia */
	gtk_builder_connect_signals(builder,NULL);
	
	
	/* Loop principale */
	gtk_main();

	return 0;
}
