#define MaxNumDomains 10
#define MaxConfigSize 10000
#define MaxFileName 100
#define MaxNumConnections 10

#define NumOfInputOptions 17

#define CONNECT 1
#define CLOSECON 2
#define DUMPXML 3
#define CREATEDOM 4
#define SUSPEND 5
#define RESUME 6
#define SAVE 7
#define RESTORE 8
#define SHUTDOWN 9
#define REBOOT 10
#define DOMINFO 11
#define NUMDOMAIN 12
#define NODEINFO 13
#define NODELIST 14
#define NODECAP 15
#define LOAD 16
#define DOMLIST 17

char textData[20];

int assignNum (char *input);
int getNextDomainThreadNum (int conNum);
void createDomain (int conNum);
void *manageDomain (void *arg);
void createConnection (int conNum);
int isConnectionEstablished (char *hostname);
int getNextConnThread ();
void printNodeInfo (virNodeInfo nodeinfo);
void printNodeList ();
int connectionWithSameURI (char *uri);
void printDomInfo (virDomainInfoPtr dominfo);
int isDomCreated (char *domName, int conNum);
void printDomList (int conNum);

gint delete_event(GtkWidget *, GdkEvent *, gpointer);
void destroy(GtkWidget *, gpointer);
void connectf(GtkWidget *, gpointer);
void createwidgets();
void printWin(char *);
int handleInput(GtkWidget *, gpointer);
void textEntry(char *, int);
void taskOnOk(GtkWidget *, gpointer);
void taskOnBack(GtkWidget *, gpointer);
void closeCon();
void createDom();
void * domainCreation(void *);
void taskOnCreate(GtkWidget *, gpointer);
char * getRadioSelected();
void insertinto(char *, char *, int);
