
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
void printWin(char *s){
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
		printWin(pr);
		return;
	}
	connection[conNum].conn = virConnectOpen (uri);
	if (connection[conNum].conn == NULL)
		sprintf (pr, "Error: Failed to open connection to %s\n\n", uri);
	else{
		connection[conNum].isconnected = 1;
		sprintf (pr, "Connection to %s established\n\n", uri);
	}
	printWin(pr);
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
    	
    	gtk_signal_connect (GTK_OBJECT (connect), "clicked",GTK_SIGNAL_FUNC (handleInput), (gpointer) CONNECT);
    	
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

int handleInput (GtkWidget *widget, gpointer button) {
	int input=(int) button;
	switch (input) {
		case CONNECT: {
			int conNum = getNextConnThread ();
			createConnection (conNum);
		}
		break;
		
	/*	case CLOSECON: {
			int isret, conNum;
			char line [50];
			fprintf (stdout, "Enter hostname: ");
			scanf ("%s", line);
			isret = isConnectionEstablished (line);
			if (isret >= 0) {
				conNum = isret;
				strcpy (line, virConnectGetHostname (connection[conNum].conn));
			}
			else {
				fprintf (stderr, "Error: Invalid connection to %s\n\n", line);
				return -1;
			}
			if (virConnectClose (connection[conNum].conn) < 0) {
				fprintf (stderr, "Error: Failed to close connection to %s\n\n", line);
				return -1;
			}
			connection[conNum].isconnected = 0;
			fprintf (stdout, "Connection to %s closed\n\n", line);
		}
		break;
		
		case NUMDOMAIN: {
			int numDomains, isret, conNum;
			char hostname [50];
			fprintf (stdout, "Enter hostname: ");
			scanf ("%s", hostname);
			isret = isConnectionEstablished (hostname);
			if (isret >= 0) {
				conNum = isret;
				strcpy (hostname, virConnectGetHostname (connection[conNum].conn));
			}
			else {
				fprintf (stderr, "Error: Invalid connection to %s\n\n", hostname);
				return -1;
			}
			numDomains = virConnectNumOfDomains (connection[conNum].conn);
			fprintf (stdout, "Total number of domains in the host %s: %d\n\n",hostname, numDomains);
		}
		break;
		
		case CREATEDOM: {
			int isret, conNum;
			char hostname [50];
			fprintf (stdout, "Enter hostname: ");
			scanf ("%s", hostname);
			isret = isConnectionEstablished (hostname);
			if (isret >= 0) {
				conNum = isret;
			}
			else {
				fprintf (stderr, "Error: Invalid connection to %s\n\n", hostname);
				return -1;
			}
			createDomain (conNum);
		}
		break;
		
		case SHUTDOWN: {
			fprintf (stdout, "Enter domain name: ");
			char domName [20];
			int isret, conNum;
			scanf ("%s", domName);
			char hostname [50];
			fprintf (stdout, "Enter hostname: ");
			scanf ("%s", hostname);
			isret = isConnectionEstablished (hostname);
			if (isret >= 0) {
				conNum = isret;
			}
			else {
				fprintf (stderr, "Error: Invalid connection to %s\n\n", hostname);
				return -1;
			}
// 			virDomainPtr dom;
			isret = isDomCreated (domName, conNum);
			if (isret < 0) {
				fprintf (stderr, "Error: Domain does not exists\n\n");
				return -1;
			}
			connection[conNum].domain[isret].iscreated = 0;
// 			dom = virDomainLookupByName (connection[conNum].conn, domName);
			isret = virDomainShutdown(connection[conNum].domain[isret].dom);
			virConnectClose (connection[conNum].conn);
			if (isret != 0) {
				fprintf (stderr, "Error: Cannot shutdown domain object\n\n");
				return -1;
			}
			fprintf (stdout, "Domain %s shutdown on %s\n\n", domName, hostname);
			if (virDomainFree (connection[conNum].domain[isret].dom) != 0) {
				fprintf (stderr, "Error: Cannot free domain datastructure\n\n");
				return -1;
			}
			fprintf (stdout, "All datastructure related to domain %s freed on %s\n\n", domName, hostname);
		}
		break;
		
		case NODEINFO: {
			int isret, conNum;
			char hostname [50];
			fprintf (stdout, "Enter hostname: ");
			scanf ("%s", hostname);
			isret = isConnectionEstablished (hostname);
			if (isret >= 0) {
				conNum = isret;
			}
			else {
				fprintf (stderr, "Error: Invalid connection to %s\n\n", hostname);
				return -1;
			}
			virNodeInfo nodeinfo;
			isret = virNodeGetInfo(connection[conNum].conn, &nodeinfo);
			if (isret != 0) {
				fprintf (stderr, "Error: Cannot get node information of %s\n\n", hostname);
				return -1;
			}
			printNodeInfo (nodeinfo);
		}
		break;
		
		case NODELIST: {
			printNodeList();
		}
		break;
		
		case NODECAP: {
			int isret, conNum;
			char hostname [50], *caps;
			fprintf (stdout, "Enter hostname: ");
			scanf ("%s", hostname);
			isret = isConnectionEstablished (hostname);
			if (isret >= 0) {
				conNum = isret;
			}
			else {
				fprintf (stderr, "Error: Invalid connection to %s\n\n", hostname);
				return -1;
			}
			caps = virConnectGetCapabilities (connection[conNum].conn);
			fprintf (stdout, "%s\n\n", caps);
		}
		break;
		
		case DUMPXML: {
			int isret, conNum;
			char name [50], *xmldump;
			fprintf (stdout, "Enter hostname: ");
			scanf ("%s", name);
			isret = isConnectionEstablished (name);
			if (isret >= 0) {
				conNum = isret;
			}
			else {
				fprintf (stderr, "Error: Invalid connection to %s\n\n", name);
				return -1;
			}
			fprintf (stdout, "Enter Domain name: ");
			scanf ("%s", name);
			virDomainPtr dom;
			dom = virDomainLookupByName (connection[conNum].conn, name);
			if (dom == NULL) {
				fprintf (stdout, "Error: Invalid domain %s\n\n", name);
				return -1;
			}
 			xmldump = virDomainGetXMLDesc (dom, VIR_DOMAIN_XML_SECURE | VIR_DOMAIN_XML_INACTIVE | VIR_DOMAIN_XML_UPDATE_CPU);
			fprintf (stdout, "%s\n\n", xmldump);
		}
		break;
		
		case REBOOT: {
			int isret, conNum;
			char name[50];
			fprintf (stdout, "Enter hostname: ");
			scanf ("%s", name);
			isret = isConnectionEstablished (name);
			if (isret >= 0) {
				conNum = isret;
			}
			else {
				fprintf (stderr, "Error: Invalid connection to %s\n\n", name);
				return -1;
			}
			fprintf (stdout, "Enter Domain name: ");
			scanf ("%s", name);
			virDomainPtr dom;
			dom = virDomainLookupByName (connection[conNum].conn, name);
			if (dom == NULL) {
				fprintf (stderr, "Error: Invalid domain %s\n\n", name);
				return -1;
			}
			
			//isret = virDomainReboot (dom, VIR_DOMAIN_REBOOT_DEFAULT);
			if (isret < 0) {
				fprintf (stderr, "Error: Cannot boot domain %s\n\n", name);
				return -1;
			}
			fprintf (stdout, "Guest domain %s ready for reboot\n\n", name);
		}
		break;
		
		case SAVE: {
			int isret, conNum;
			char name[50];
			fprintf (stdout, "Enter hostname: ");
			scanf ("%s", name);
			isret = isConnectionEstablished (name);
			if (isret >= 0) {
				conNum = isret;
			}
			else {
				fprintf (stderr, "Error: Invalid connection to %s\n\n", name);
				return -1;
			}
			fprintf (stdout, "Enter Domain name: ");
			scanf ("%s", name);
			virDomainPtr dom;
			dom = virDomainLookupByName (connection[conNum].conn, name);
			if (dom == NULL) {
				fprintf (stderr, "Error: Invalid domain %s\n\n", name);
				return -1;
			}
			fprintf (stdout, "Enter filename to be saved as: ");
			scanf ("%s", name);
			isret = virDomainSave (dom, name);
			if (isret < 0) {
				fprintf (stderr, "Error: Cannot save domain memory\n\n");
				return -1;
			}
			fprintf (stdout, "Domain memory saved\n\n");
		}
		break;
		
		case RESTORE: {
			int isret, conNum;
			char name[50];
			fprintf (stdout, "Enter hostname: ");
			scanf ("%s", name);
			isret = isConnectionEstablished (name);
			if (isret >= 0) {
				conNum = isret;
			}
			else {
				fprintf (stderr, "Error: Invalid connection to %s\n\n", name);
				return -1;
			}
			fprintf (stdout, "Enter filename for restoring: ");
			scanf ("%s", name);
			isret = virDomainRestore (connection[conNum].conn, name);
			if (isret < 0) {
				fprintf (stderr, "Error: Cannot restore domain memory\n\n");
				return -1;
			}
			fprintf (stdout, "Domain memory restored from %s to %s\n\n", name, virConnectGetHostname (connection[conNum].conn));
		}
		break;
		
		case SUSPEND: {
			int isret, conNum;
			char name[50];
			fprintf (stdout, "Enter hostname: ");
			scanf ("%s", name);
			isret = isConnectionEstablished (name);
			if (isret >= 0) {
				conNum = isret;
			}
			else {
				fprintf (stderr, "Error: Invalid connection to %s\n\n", name);
				return -1;
			}
			fprintf (stdout, "Enter Domain name: ");
			scanf ("%s", name);
			virDomainPtr dom;
			dom = virDomainLookupByName (connection[conNum].conn, name);
			if (dom == NULL) {
				fprintf (stderr, "Error: Invalid domain %s\n\n", name);
				return -1;
			}
			isret = virDomainSuspend (dom);
			if (isret != 0) {
				fprintf (stderr, "Error: Domain %s cannot be suspended, check privileges\n\n", name);
				return -1;
			}
			fprintf (stdout, "Domain %s suspended. Memory still allocated\n\n", name);
		}
		break;
		
		case RESUME: {
			int isret, conNum;
			char name[50];
			fprintf (stdout, "Enter hostname: ");
			scanf ("%s", name);
			isret = isConnectionEstablished (name);
			if (isret >= 0) {
				conNum = isret;
			}
			else {
				fprintf (stderr, "Error: Invalid connection to %s\n\n", name);
				return -1;
			}
			fprintf (stdout, "Enter Domain name: ");
			scanf ("%s", name);
			virDomainPtr dom;
			dom = virDomainLookupByName (connection[conNum].conn, name);
			if (dom == NULL) {
				fprintf (stderr, "Error: Invalid domain %s\n\n", name);
				return -1;
			}
			isret = virDomainResume (dom);
			if (isret != 0) {
				fprintf (stderr, "Error: Domain %s cannot be restored\n\n", name);
				return -1;
			}
			fprintf (stdout, "Domain %s restored. Hypervisor resources available\n\n", name);
		}
		break;
		
		case LOAD: {
			int isret, conNum;
			char name[50], *net[30]; 
			int size = 30;
			fprintf (stdout, "Enter hostname: ");
			scanf ("%s", name);
			isret = isConnectionEstablished (name);
			if (isret >= 0) {
				conNum = isret;
			}
			else {
				fprintf (stderr, "Error: Invalid connection to %s\n\n", name);
				return -1;
			}
			isret = virConnectListNetworks (connection[conNum].conn, net, size);
			fprintf (stdout, "Number of networks in %s: %d\n\n",name, isret);
		}
		break;
		
		case DOMINFO: {
			int isret, conNum;
			char name[50]; 
			fprintf (stdout, "Enter hostname: ");
			scanf ("%s", name);
			isret = isConnectionEstablished (name);
			if (isret >= 0) {
				conNum = isret;
			}
			else {
				fprintf (stderr, "Error: Invalid connection to %s\n\n", name);
				return -1;
			}
			fprintf (stdout, "Enter Domain name: ");
			scanf ("%s", name);
			virDomainPtr dom;
			dom = virDomainLookupByName (connection[conNum].conn, name);
			if (dom == NULL) {
				fprintf (stderr, "Error: Invalid domain %s\n\n", name);
				return -1;
			}
			virDomainInfoPtr dominfo;
			dominfo = malloc (sizeof (virDomainInfoPtr));
			isret = virDomainGetInfo (dom, dominfo);
			if (isret != 0) {
				fprintf (stderr, "Error: Cannot get info about domain %s\n\n", name);
				return -1;
			}
			printDomInfo (dominfo);
		}
		break;
		
		case DOMLIST: {
			int isret, conNum;
			char name[50]; 
			fprintf (stdout, "Enter hostname: ");
			scanf ("%s", name);
			isret = isConnectionEstablished (name);
			if (isret >= 0) {
				conNum = isret;
			}
			else {
				fprintf (stderr, "Error: Invalid connection to %s\n\n", name);
				return -1;
			}
			printDomList(conNum);
		}
		break;*/
		
		default:
			fprintf (stderr, "Error: Invalid input\n\n");
		break;
	}
	return 0;
}
