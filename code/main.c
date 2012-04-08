#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <libvirt/libvirt.h>			// INCLUDING THE LIBVIRT LIBRARY
#include <pthread.h>
#include <assert.h>
#include <string.h>
#include "definitions.h"


/* -MAIN- */
int main (int argc, char *argv[]) {
	gtk_init (&argc, &argv);
	
    	createwidgets();

    	return(0);
}

