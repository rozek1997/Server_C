//
// Created by xbox3 on 26.04.2019.
//

#ifndef SERVER_C_PROPERTIES_LOADER_H
#define SERVER_C_PROPERTIES_LOADER_H
#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<string.h>

/*
 * simple struct store property
 */
struct map{
    char  key[50];
    char  value[50];
};


void setProperty(char * key, char * value, int position);
char * getProperty(char * key);
int findInString(char * string, char  sign);
int tokenizer(struct map* token, char * stringToMap);
void loadPropertyFile(const char * filePath);

#endif //SERVER_C_PROPERTIES_LOADER_H
