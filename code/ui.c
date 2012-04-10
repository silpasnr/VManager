
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
	GtkWidget *back;
	GtkWidget *ok;
	GtkWidget *new;	
	GtkWidget *table;
	GtkWidget *window;
	int flag;
};
struct widgets buttons;
	struct widgets install;

gint delete_event(GtkWidget *widget, GdkEvent *event, gpointer data){
    	gtk_main_quit();
    	return(FALSE);
}

void destroy( GtkWidget *widget, gpointer data ){
    	gtk_main_quit();
}

char * getRadioSelected(){
    	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(install.wid[4]))==TRUE) return("cdrom");
    	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(install.wid[5]))==TRUE) return("");
    	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(install.wid[6]))==TRUE) return("");
}

void insertinto(char *file, char *string, int loc){
	int strLength, fileLength;
	char c;
	FILE *fp1;
	fp1=fopen(file, "r+");
	strLength=strlen(string);
	fseek(fp1, 0L, SEEK_END);
	for (fileLength = (int)ftell(fp1); fileLength>=loc; fileLength--){
		fseek(fp1, (fileLength-1), SEEK_SET);
		c=fgetc(fp1);
		fseek(fp1, (fileLength+strLength-1), SEEK_SET);
		fputc(c, fp1);
	}	
	fseek(fp1, loc, SEEK_SET);
	fprintf(fp1, string);
	fclose(fp1);
}

void taskOnCreate(GtkWidget *widget, gpointer data){
	char dev[10];
	char name[10];
	char arch[10];
	char domain[10];
	char vcpu[10];
	char *sourceFile="/home/delbin/VManager/code/ubuntu.iso";
	strcpy(dev, getRadioSelected());
	strcpy(name, gtk_entry_get_text(GTK_ENTRY (install.wid[1])));	
	strcpy(vcpu, gtk_entry_get_text(GTK_ENTRY (install.wid[9])));
    	strcpy(arch, gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(install.wid[2])->entry)));
    	strcpy(domain, gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(install.wid[3])->entry)));			
		
	gtk_widget_hide(install.wid[0]);
	gtk_widget_hide(install.wid[7]);	
	gtk_widget_hide(install.wid[8]);	
	insertinto("coden", sourceFile, 365);
	insertinto("coden", dev, 231);	
	insertinto("coden", arch, 190);	
	insertinto("coden", vcpu, 159);
	insertinto("coden", name, 25);			
	insertinto("coden", domain, 14);		
	g_print("%s\t%s\t%s\t%s\t%s",dev, name, vcpu, arch, domain);
	}
void *domainCreation(void *arg){
	//gtk_widget_hide(buttons.new);
	//gtk_widget_hide(buttons.ok);	
	int i;
		for (i=1; i<=8; i++)
		gtk_widget_hide(buttons.wid[i]);
	
	GtkWidget *table;
	GtkWidget *title;		
	GtkWidget *nameLabel;	
	GtkWidget *nameField;
	GtkWidget *archLabel;
	GtkWidget *archMenu;
	GtkWidget *hypLabel;
	GtkWidget *hypMenu;
	GtkWidget *devLabel;
	GtkWidget *devRadio1;
	GtkWidget *devRadio2;
	GtkWidget *devRadio3;		
	GtkWidget *locLabel;
	GtkWidget *fileBrowse;
	GtkWidget *vcpuLabel;
	GtkWidget *vcpuField;
	GtkWidget *create;	
	GSList *radio;

	
 	table = gtk_table_new (8, 2,TRUE);	
	
	GList *archs = NULL;
	archs = g_list_append (archs, "1686");
	archs = g_list_append (archs, "x86_64");
	archs = g_list_append (archs, "mips");
	archs = g_list_append (archs, "mipsel");
	archs = g_list_append (archs, "sparc");
	archs = g_list_append (archs, "ppc");
	archMenu = gtk_combo_new ();
	gtk_combo_set_popdown_strings (GTK_COMBO (archMenu), archs);

	GList *hypervisor = NULL;
	hypervisor = g_list_append (hypervisor, "qemu");
	hypMenu = gtk_combo_new ();
	gtk_combo_set_popdown_strings (GTK_COMBO (hypMenu), hypervisor);
	
	nameField = gtk_entry_new ();  
	vcpuField = gtk_entry_new ();  
       	title = gtk_label_new ("INSTALLATION WIZARD");   	
       	nameLabel = gtk_label_new ("Name");       	
       	archLabel = gtk_label_new ("Architecture");       	
       	hypLabel = gtk_label_new ("Hypervisor");       	
       	devLabel = gtk_label_new ("Installation Media");   
       	locLabel = gtk_label_new ("Location");      	
       	vcpuLabel = gtk_label_new ("Virtual CPU's");       	
    	create = gtk_button_new_with_label ("CREATE");       	
       	
    	devRadio1 = gtk_radio_button_new_with_label (NULL,"ISO image or CDROM");   
     	gtk_table_attach_defaults (GTK_TABLE(table), devRadio1, 1, 2, 3, 4);         	    	
    	radio = gtk_radio_button_group (GTK_RADIO_BUTTON (devRadio1));
    	
    	gtk_widget_show (devRadio1);
    	devRadio2 = gtk_radio_button_new_with_label(radio, "HTTP, FTP or NFS");
    	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (devRadio1), TRUE);
     	gtk_table_attach_defaults (GTK_TABLE(table), devRadio2, 1, 2, 4, 5);         	
    	gtk_widget_show (devRadio2);
    	devRadio3 = gtk_radio_button_new_with_label(
                 gtk_radio_button_group (GTK_RADIO_BUTTON (devRadio2)),
                 "Network Boot (PXE)");
        gtk_widget_show (devRadio3);
        
    	gtk_signal_connect (GTK_OBJECT (create), "clicked",GTK_SIGNAL_FUNC (taskOnCreate), (gpointer) CONNECT);        

       	gtk_box_pack_end(GTK_BOX(buttons.wid[0]), create, TRUE, TRUE, 10);      	
       	gtk_box_pack_end(GTK_BOX(buttons.wid[0]), table, TRUE, TRUE, 0);     
       	gtk_box_pack_end(GTK_BOX(buttons.wid[0]), title, TRUE, TRUE, 0);            		
	gtk_table_attach_defaults (GTK_TABLE(table), nameLabel, 0, 1, 0, 1);
        gtk_table_attach_defaults (GTK_TABLE(table), nameField, 1, 2, 0, 1);	 	
    	gtk_table_attach_defaults (GTK_TABLE(table), archLabel, 0, 1, 1, 2);
    	gtk_table_attach_defaults (GTK_TABLE(table), archMenu, 1, 2, 1, 2);    	        
     	gtk_table_attach_defaults (GTK_TABLE(table), hypLabel, 0, 1, 2, 3);    	
     	gtk_table_attach_defaults (GTK_TABLE(table), hypMenu, 1, 2, 2, 3);
     	gtk_table_attach_defaults (GTK_TABLE(table), devLabel, 0, 1, 3, 6);   
     	gtk_table_attach_defaults (GTK_TABLE(table), locLabel, 0, 1, 6, 7);     	
     	gtk_table_attach_defaults (GTK_TABLE(table), devRadio3, 1, 2, 5, 6);        	
    	gtk_table_attach_defaults (GTK_TABLE(table), vcpuLabel, 0, 1, 7, 8);     	     	
    	gtk_table_attach_defaults (GTK_TABLE(table), vcpuField, 1, 2, 7, 8);    	
    	gtk_widget_show (nameLabel);
    	gtk_widget_show (nameField);
    	gtk_widget_show (archLabel);
    	gtk_widget_show (archMenu);
    	gtk_widget_show (hypLabel);
    	gtk_widget_show (hypMenu);
    	gtk_widget_show (devLabel);    	
        gtk_widget_show (locLabel);  	
        gtk_widget_show (devLabel);  	        
    	gtk_widget_show (vcpuLabel);
    	gtk_widget_show (vcpuField);
    	gtk_widget_show (title);    	
    	gtk_widget_show (table);    		    	  
    	gtk_widget_show (create);  	  	
    	
    	install.wid[0]=table;
    	install.wid[1]=nameField;
    	install.wid[2]=archMenu;
    	install.wid[3]=hypMenu;
    	install.wid[4]=devRadio1;
    	install.wid[5]=devRadio2;
    	install.wid[6]=devRadio3;    
    	install.wid[7]=create;
	install.wid[8]=title;    
	install.wid[9]=vcpuField;		
    	//install
    	manageDomain(arg);
    	    	    	    	    	
 }
void textEntry(char *display, int ref){
	int i;	
	for (i=1; i<=8; i++)
		gtk_widget_hide(buttons.wid[i]);	
	GtkWidget *text;    	
	text = gtk_entry_new ();  
       	gtk_entry_set_text (GTK_ENTRY (text), display);
 	g_signal_connect (text, "activate",
		      G_CALLBACK (taskOnOk),
		      (gpointer) ref);     	
       	gtk_box_pack_end(GTK_BOX(buttons.wid[0]), text, TRUE, TRUE, 0);
	gtk_widget_show(text);
       	gtk_widget_show(buttons.ok);
       	buttons.new=text;
       	buttons.flag=ref;
}

void printWin(char *s){
	int i;	
	for (i=1; i<=8; i++)
		gtk_widget_hide(buttons.wid[i]);
	
	GtkWidget *label;    	
       	label = gtk_label_new (s);
       	gtk_box_pack_end(GTK_BOX (buttons.wid[0]), label, TRUE, TRUE, 0);	
       	gtk_widget_show(label);
       	gtk_widget_show(buttons.wid[9]);
       	buttons.new=label;
}


void taskOnBack( GtkWidget *widget, gpointer   data ){
	int i;
	gtk_widget_hide(buttons.new);
  	gtk_widget_destroy(buttons.new);
	gtk_widget_hide(buttons.back);  	
  	for (i=1; i<=8; i++)
		gtk_widget_show(buttons.wid[i]);
}

void taskOnOk( GtkWidget *widget, gpointer data ){
	bzero(textData, 20);
	strcpy(textData, gtk_entry_get_text(GTK_ENTRY (buttons.new)));
  	gtk_widget_destroy(buttons.new);
	gtk_widget_hide(buttons.ok);  
	gtk_widget_show(buttons.back);
	switch (buttons.flag){
		case CONNECT :{
			int conNum = getNextConnThread();		
			createConnection(conNum);
		}
			break;
		case CLOSECON : closeCon();
			break;
		case CREATEDOM : createDom();
			break;
			
		default :printWin("Invalid input");
	}
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

	//gtk_widget_set_size_request( GTK_WIDGET(back), 10, 10 );
	//gtk_widget_set_size_request( GTK_WIDGET(ok), 10, 5 );	
                                      	     	 	
    	buttons.wid[0]=box;    	     	 	
    	buttons.wid[1]=connect;
    	buttons.wid[2]=closecon;
    	buttons.wid[3]=numdomain;
    	buttons.wid[4]=createdom;    
    	buttons.wid[5]=shutdown;
    	buttons.wid[6]=nodeinfo;
    	buttons.wid[7]=nodelist;       	        	    	
    	buttons.wid[8]=quit;
    	buttons.back=back;   
    	buttons.ok=ok; 	
    	buttons.window=window;
    	
    	gtk_signal_connect (GTK_OBJECT (connect), "clicked",GTK_SIGNAL_FUNC (handleInput), (gpointer) CONNECT);
    	gtk_signal_connect (GTK_OBJECT (closecon), "clicked",GTK_SIGNAL_FUNC (handleInput), (gpointer) CLOSECON);
    	gtk_signal_connect (GTK_OBJECT (numdomain), "clicked",GTK_SIGNAL_FUNC (handleInput), (gpointer) NUMDOMAIN);
    	gtk_signal_connect (GTK_OBJECT (createdom), "clicked",GTK_SIGNAL_FUNC (handleInput), (gpointer) CREATEDOM);
    	gtk_signal_connect (GTK_OBJECT (shutdown), "clicked",GTK_SIGNAL_FUNC (handleInput), (gpointer) SHUTDOWN);
    	gtk_signal_connect (GTK_OBJECT (nodeinfo), "clicked",GTK_SIGNAL_FUNC (handleInput), (gpointer) NODEINFO);
    	gtk_signal_connect (GTK_OBJECT (nodelist), "clicked",GTK_SIGNAL_FUNC (handleInput), (gpointer) NODELIST); 	
    	
    	gtk_signal_connect (GTK_OBJECT (ok), "clicked",GTK_SIGNAL_FUNC (taskOnOk), (gpointer) "ok");       
        gtk_signal_connect (GTK_OBJECT (back), "clicked",GTK_SIGNAL_FUNC (taskOnBack), (gpointer) "back");
        
        gtk_signal_connect (GTK_OBJECT (quit), "clicked",
        GTK_SIGNAL_FUNC (destroy), (gpointer) "exit");

       	gtk_box_pack_end(GTK_BOX(box), ok, TRUE, TRUE, 50);
       	gtk_box_pack_end(GTK_BOX(box), quit, TRUE, TRUE, 0);
       	gtk_box_pack_end(GTK_BOX(box), back, TRUE, TRUE, 0);  		gtk_box_pack_end(GTK_BOX(box), nodelist, TRUE, TRUE, 0);
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
