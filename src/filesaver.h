//
// Created by xbox3 on 25.04.2019.
//

#ifndef SERVER_C_FILESAVER_H
#define SERVER_C_FILESAVER_H
#define BUFFER_SIZE 6000
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<string.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include<pthread.h>
#include<json-c/json.h>
#include <sys/types.h>
#include <sys/stat.h>
struct messageStruct{

    char broadcasterID[100];
    char data[BUFFER_SIZE];
};


void convertJsonToStruct(char* json, struct messageStruct * structdata_to_save);
void saveStructToFile(struct messageStruct *structDataToSave, FILE *fp);
int saveStringToFile(char * message, FILE *fp);


#endif //SERVER_C_FILESAVER_H
