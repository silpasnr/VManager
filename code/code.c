
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <libvirt/libvirt.h>		// INCLUDING THE LIBVIRT LIBRARY
#include <pthread.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include "definitions.h"

int globalConHandler;

char inputOptions[][NumOfInputOptions] = {"connect", "close", "dumpxml", "createdom", "suspend", "resume", "save", "restore", "shutdown", "reboot", "dominfo", "numdomain", "nodeinfo", "nodelist", "nodecap", "load", "domlist"};

struct connThreadStruct {
	pthread_t domainThread [MaxNumDomains];
}cThread[MaxNumConnections];

struct domStruct {
	virDomainPtr dom;
	int iscreated;
};

struct connStruct {
	virConnectPtr conn;
	int isconnected;
	struct domStruct domain [MaxNumDomains];
	int numGuestDomains;
}connection[MaxNumConnections];

struct widgets{
	GtkWidget *wid[4];
	GtkWidget *new;
};
struct widgets buttons;
void connectf(GtkWidget *, gpointer);
void backf(GtkWidget *, gpointer);
void createwidgets();


gint delete_event(GtkWidget *widget, GdkEvent *event, gpointer data){
    	gtk_main_quit();
    	return(FALSE);
}

void destroy( GtkWidget *widget, gpointer data ){
    	gtk_main_quit();
}

int main( int   argc, char *argv[] ){
    	gtk_init (&argc, &argv);
//	GtkWidget **wid;
    	createwidgets();

    	return(0);
}

int connectionWithSameURI (char *uri) {
	int i;
	for (i=0; i < MaxNumConnections; i++) {
		if (connection[i].isconnected == 1)
			if (!strcmp(virConnectGetURI(connection[i].conn), uri)) {
				return i;
			}
	}
	return -1;
}
void printwn(char *s){
	int i;	
	for (i=1; i<=2; i++)
		gtk_widget_hide(buttons.wid[i]);
	
	GtkWidget *label;    	
       	label = gtk_label_new (s);
       	gtk_box_pack_end(GTK_BOX(buttons.wid[0]), label, TRUE, TRUE, 0);	
       	gtk_widget_show(label);
       	gtk_widget_show(buttons.wid[3]);
       	buttons.new=label;
}
void createConnection (int conNum) {
	char uri[50];
	char pr[100];
	fprintf (stdout, "Enter URI: ");
	scanf ("%s", uri);
	if (connectionWithSameURI (uri) != -1) {
		sprintf (pr, "Error: Connection to %s already exists\n\n", uri);
		//printwn(pr);
		return;
	}
	connection[conNum].conn = virConnectOpen (uri);
	if (connection[conNum].conn == NULL)
		sprintf (pr, "Error: Failed to open connection to %s\n\n", uri);
	else{
		connection[conNum].isconnected = 1;
		sprintf (pr, "Connection to %s established\n\n", uri);
	}
	//printwn(pr);
	return;
}

int getNextConnThread () {
	int i;
	for (i=0; i < MaxNumConnections; i++) {
		if (connection[i].conn == NULL) {
			return i;
		}
	}
	return -1;
}

void connectf( GtkWidget *widget, gpointer data) {

	int conNum = getNextConnThread ();
	createConnection (conNum);
	
}

void backf( GtkWidget *widget, gpointer   data ){
	int i;
  	gtk_widget_destroy(buttons.new);
	gtk_widget_hide(buttons.wid[3]);  	
  	for (i=1; i<=2; i++)
		gtk_widget_show(buttons.wid[i]);
  	g_print ("Yay!\n");
}

void createwidgets(){
    	GtkWidget *window;
    	GtkWidget *connect;
    	GtkWidget *quit;
    	GtkWidget *back;    	
    	GtkWidget *box;
    	    	
    	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    	gtk_window_set_title (GTK_WINDOW (window), "VManager");
    	gtk_signal_connect (GTK_OBJECT (window), "delete_event",
                        GTK_SIGNAL_FUNC (delete_event), NULL);
    	gtk_container_set_border_width (GTK_CONTAINER (window), 100);

    	box = gtk_vbox_new(FALSE, 10);

    	gtk_container_add (GTK_CONTAINER (window), box);
    
    	connect = gtk_button_new_with_label ("Connect");
    	quit = gtk_button_new_with_label ("Quit");   
    	back = gtk_button_new_with_label ("Back");
    	     	 	
    	buttons.wid[0]=box;    	     	 	
    	buttons.wid[1]=connect;
    	buttons.wid[2]=quit;
    	buttons.wid[3]=back;    	
    	
    	gtk_signal_connect (GTK_OBJECT (connect), "clicked",GTK_SIGNAL_FUNC (connectf), (gpointer) buttons.wid);
    	
    	gtk_signal_connect (GTK_OBJECT (quit), "clicked",
        GTK_SIGNAL_FUNC (destroy), (gpointer) "exit");
        
        gtk_signal_connect (GTK_OBJECT (back), "clicked",GTK_SIGNAL_FUNC (backf), (gpointer) "back");

       	gtk_box_pack_end(GTK_BOX(box), quit, TRUE, TRUE, 0);
       	gtk_box_pack_end(GTK_BOX(box), back, TRUE, TRUE, 0);      	
       	gtk_box_pack_end(GTK_BOX(box), connect, TRUE, TRUE, 0);

     	    	
    	gtk_widget_show(connect);
    	gtk_widget_show(quit);
    //	gtk_widget_show(back);
    	gtk_widget_show(box);    	    	
    	gtk_widget_show (window);
    	gtk_main ();
    	//return (buttons->wid);

}

