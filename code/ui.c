
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

struct widgets{
	GtkWidget *wid[10];
	GtkWidget *new;
	GtkWidget *text;
	GtkWidget *ok;
	int flag;
};
struct widgets buttons;

gint delete_event(GtkWidget *widget, GdkEvent *event, gpointer data){
    	gtk_main_quit();
    	return(FALSE);
}

void destroy( GtkWidget *widget, gpointer data ){
    	gtk_main_quit();
}

void getUri(){
	int i;	
	for (i=1; i<=8; i++)
		gtk_widget_hide(buttons.wid[i]);	
	GtkWidget *text;    	
	text = gtk_entry_new ();  
       	gtk_entry_set_text (GTK_ENTRY (text), "Enter the URI here");
     	
       	gtk_box_pack_end(GTK_BOX(buttons.wid[0]), text, TRUE, TRUE, 0);	
       	gtk_widget_show(text);
       	gtk_widget_show(buttons.ok);
       	buttons.new=text;
       	buttons.text=text;
       	buttons.flag=1;
}

void printWin(char *s){
	int i;	
	for (i=1; i<=8; i++)
		gtk_widget_hide(buttons.wid[i]);
	
	GtkWidget *label;    	
       	label = gtk_label_new (s);
       	gtk_box_pack_end(GTK_BOX(buttons.wid[0]), label, TRUE, TRUE, 0);	
       	gtk_widget_show(label);
       	gtk_widget_show(buttons.wid[9]);
       	buttons.new=label;
}


void backf( GtkWidget *widget, gpointer   data ){
	int i;
  	gtk_widget_destroy(buttons.new);
	gtk_widget_hide(buttons.wid[9]);  	
  	for (i=1; i<=8; i++)
		gtk_widget_show(buttons.wid[i]);

  	//g_print ("Yay!\n");
}

void okf( GtkWidget *widget, gpointer data ){
	bzero(textData, 20);
	strcpy(textData, gtk_entry_get_text(GTK_ENTRY (buttons.new)));
  	gtk_widget_destroy(buttons.new);
	gtk_widget_hide(buttons.ok);  
	if (buttons.flag==1){
		int conNum = getNextConnThread ();		
		createConnection(conNum);
		buttons.flag=0;
	}
  //	for (i=1; i<=8; i++)
//		gtk_widget_show(buttons.wid[i]);
  	//g_print ("Yay!\n");
}

void createwidgets(){
    	GtkWidget *window;
    	GtkWidget *connect;
    	GtkWidget *closecon;
    	GtkWidget *numdomain;
    	GtkWidget *createdom;
    	GtkWidget *shutdown;
    	GtkWidget *nodeinfo;
    	GtkWidget *nodelist;    	    	    	
    	GtkWidget *quit;
    	GtkWidget *back;    	
    	GtkWidget *box;
    	GtkWidget *ok;    	
    	    	
    	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    	gtk_window_set_title (GTK_WINDOW (window), "VManager");
    	gtk_signal_connect (GTK_OBJECT (window), "delete_event",
                        GTK_SIGNAL_FUNC (delete_event), NULL);
    	gtk_container_set_border_width (GTK_CONTAINER (window), 100);

    	box = gtk_vbox_new(FALSE, 10);

    	gtk_container_add (GTK_CONTAINER (window), box);
    
    	connect = gtk_button_new_with_label ("Connect");
    	closecon = gtk_button_new_with_label ("Close Connection");        	 
    	numdomain = gtk_button_new_with_label ("Number of Domains");
    	createdom = gtk_button_new_with_label ("Create Domain");
    	shutdown = gtk_button_new_with_label ("Shutdown");    	
    	nodeinfo = gtk_button_new_with_label ("nodeinfo");
    	nodelist = gtk_button_new_with_label ("nodelist");
    	quit = gtk_button_new_with_label ("Quit");   
    	back = gtk_button_new_with_label ("Back");
    	ok = gtk_button_new_with_label ("Ok");    	

	gtk_widget_set_size_request( GTK_WIDGET(back), 30, 30 );
	gtk_widget_set_size_request( GTK_WIDGET(ok), 30, 30 );	
                                      	     	 	
    	buttons.wid[0]=box;    	     	 	
    	buttons.wid[1]=connect;
    	buttons.wid[2]=closecon;
    	buttons.wid[3]=numdomain;
    	buttons.wid[4]=createdom;    
    	buttons.wid[5]=shutdown;
    	buttons.wid[6]=nodeinfo;
    	buttons.wid[7]=nodelist;       	        	    	
    	buttons.wid[8]=quit;
    	buttons.wid[9]=back;   
    	buttons.ok=ok; 	
    	
    	gtk_signal_connect (GTK_OBJECT (connect), "clicked",GTK_SIGNAL_FUNC (handleInput), (gpointer) CONNECT);
    	gtk_signal_connect (GTK_OBJECT (closecon), "clicked",GTK_SIGNAL_FUNC (handleInput), (gpointer) CLOSECON);
    	gtk_signal_connect (GTK_OBJECT (numdomain), "clicked",GTK_SIGNAL_FUNC (handleInput), (gpointer) NUMDOMAIN);
    	gtk_signal_connect (GTK_OBJECT (createdom), "clicked",GTK_SIGNAL_FUNC (handleInput), (gpointer) CREATEDOM);
    	gtk_signal_connect (GTK_OBJECT (shutdown), "clicked",GTK_SIGNAL_FUNC (handleInput), (gpointer) SHUTDOWN);
    	gtk_signal_connect (GTK_OBJECT (nodeinfo), "clicked",GTK_SIGNAL_FUNC (handleInput), (gpointer) NODEINFO);
    	gtk_signal_connect (GTK_OBJECT (nodelist), "clicked",GTK_SIGNAL_FUNC (handleInput), (gpointer) NODELIST); 	
    	gtk_signal_connect (GTK_OBJECT (ok), "clicked",GTK_SIGNAL_FUNC (okf), (gpointer) NODELIST);  	    	    	    	    	
    	gtk_signal_connect (GTK_OBJECT (quit), "clicked",
        GTK_SIGNAL_FUNC (destroy), (gpointer) "exit");
        
        gtk_signal_connect (GTK_OBJECT (back), "clicked",GTK_SIGNAL_FUNC (backf), (gpointer) "back");

       	gtk_box_pack_end(GTK_BOX(box), ok, TRUE, TRUE, 0);
       	gtk_box_pack_end(GTK_BOX(box), quit, TRUE, TRUE, 0);
       	gtk_box_pack_end(GTK_BOX(box), back, TRUE, TRUE, 0);   		gtk_box_pack_end(GTK_BOX(box), nodelist, TRUE, TRUE, 0);
       	gtk_box_pack_end(GTK_BOX(box), nodeinfo, TRUE, TRUE, 0);
       	gtk_box_pack_end(GTK_BOX(box), shutdown, TRUE, TRUE, 0);
       	gtk_box_pack_end(GTK_BOX(box), createdom, TRUE, TRUE, 0);
       	gtk_box_pack_end(GTK_BOX(box), numdomain, TRUE, TRUE, 0);
       	gtk_box_pack_end(GTK_BOX(box), closecon, TRUE, TRUE, 0);    	
       	gtk_box_pack_end(GTK_BOX(box), connect, TRUE, TRUE, 0);

     	    	
    	gtk_widget_show(connect);
    	gtk_widget_show(closecon);
    	gtk_widget_show(numdomain);
    	gtk_widget_show(createdom);
    	gtk_widget_show(shutdown);
    	gtk_widget_show(nodeinfo);
    	gtk_widget_show(nodelist);    	    	    	    	    	    	
    	gtk_widget_show(quit);
    	gtk_widget_show(box);    	    	
    	gtk_widget_show (window);
    	gtk_main ();

}