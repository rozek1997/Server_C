#include "filesaver.h"
#include "properties_loader.h"


void *socketThread(void *arg);

void initServer();

void activateConnection();

void acceptConnection();

void readCommandLineParameters(int c, char **argv);

const char * FILE_PATH="./data_files/";  //directory path where all recevied will be saved
char client_message[BUFFER_SIZE]; //buffer for reciving message
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int serverSocketDesc; //socket for listening
int newSocketDesc; //socket desciptor of new connection
struct sockaddr_in serverAddress;
struct sockaddr_storage serverStorage;
socklen_t addr_size;

int main(int c, char **argv) {


    readCommandLineParameters(c, argv);
    initServer();
    activateConnection();
    acceptConnection();


    return 0;
}


/*
 * every new thread invoke this function for reading from socket
 */
void *socketThread(void *arg) {
    int newSocket = *((int *) arg); //socket descriptor for this connection
    int readSize;
    int bytesSaved;

//    pthread_mutex_lock(&lock);
    char fileName[]="./data_files/apiXXXXXX";
    int fd = mkostemp(fileName, O_APPEND); //creating file with uniq name
    FILE * fp=fdopen(fd, "a");
    if(fp==NULL){ //stop executing thread if file is not opened
        fprintf(stderr, "Error creating file");
//        pthread_mutex_unlock(&lock);
        pthread_exit(0);
    }

    printf("Starting reading from socket\n");
    while ((readSize = recv(newSocket, client_message, BUFFER_SIZE, 0)) > 0) {

        client_message[readSize]='\0';
//        convertJsonToStruct(client_message, &data);
//        saveStructToFile(&data, fp);
        bytesSaved=saveStringToFile(client_message, fp);
        memset(client_message, '\0', BUFFER_SIZE);
        if(bytesSaved<readSize){ //
            fprintf(stderr, "Error saving");
        }

    }
    fclose(fp);
//    pthread_mutex_unlock(&lock);
    sleep(1);
    if (readSize == 0) {
        puts("Client disconnected\n");
        fflush(stdout);
    } else if (readSize == -1) {
        perror("recv failed");
        fprintf(stderr, "recv failed\n");
    }
    close(newSocket);
    pthread_exit(NULL);
}


//Create the socket.
// Configure settings of the server address struct
// Address family = Internet
//Set port number, using htons function to use proper byte order
//Set IP address to public ip
//Set all bits of the padding field to 0
void initServer() {
    serverSocketDesc = socket(PF_INET, SOCK_STREAM, 0);
    if (serverSocketDesc == -1) {
        fprintf(stderr, "Could not create socket\n");
        exit(1);
    }

    char *portString = getProperty("port");
    if (portString == NULL) {
        fprintf(stderr, "No property \"port\" in property file");
        exit(1);
    }

    long port = strtol(portString, NULL, 10);
    printf("Port: %ld\n", port);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    //Bind the address struct to the socket
    memset(serverAddress.sin_zero, '\0', sizeof serverAddress.sin_zero);
    //mkdir for received files
    struct stat st = {0};
    if (stat(FILE_PATH, &st) == -1) {
        mkdir(FILE_PATH, 0777);
    }
}

/*
 *  assigns the address specified by serverAdress to the socket referred to by the file descriptor serverSocketDesc.
 *  marks the socket referred to by serverSocketDesc as a passive socket, that is,
 *  50 in listen funtion defines the maximum length to which the queue of
       pending connections for serverSocketDesc may grow.
 */
void activateConnection() {
    if (bind(serverSocketDesc, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        //print the error messageStruct)
        fprintf(stderr, "bind failed. Error\n");
        return;
    }
    //Listen on the socket, with 40 max connection requests queued
    if (listen(serverSocketDesc, 50) == 0)
        printf("Listening\n");
    else
        printf("Error\n");


}

/*
 * It extracts the first connection
       request on the queue of pending connections for the listening socket,
       serverSocketDesc, creates a new connected socket, and returns a new file
       descriptor referring to that socket.
 */
void acceptConnection() {
    pthread_t tid[60];
    int i = 0;
    while (1) {
        //Accept call creates a new socket for the incoming connection
        addr_size = sizeof serverStorage;
        newSocketDesc = accept(serverSocketDesc, (struct sockaddr *) &serverStorage, &addr_size);
        //for each client request creates a thread and assign the client request to it to process
        //so the main thread can entertain next request
        if (pthread_create(&tid[i], NULL, socketThread, &newSocketDesc) != 0)
            fprintf(stderr, "Failed to create thread\n");
        if (i >= 50) {
            i = 0;
            while (i < 50) {
                pthread_join(tid[i++], NULL);

            }
            i = 0;
        }
    }
}


/*
 * read parametrs from stdin
 */
void readCommandLineParameters(int c, char **argv) {

    if (c > 1) {

        if (strcmp(argv[1], "-pf") != 0) {
            fprintf(stderr, "Use -pf \"path to properties file\"\n");
            exit(1);
        }

        loadPropertyFile(argv[2]);

    } else {
        fprintf(stderr, "Too little parameters\n");
        fprintf(stderr, "Use -pf \"path to properties file\"\n");
        exit(1);
    }
}
