//
// Created by xbox3 on 26.04.2019.
//

#include "properties_loader.h"
#define INITIAL_MAP_CAPACITY 5 //for our purpose property map hava capacity of 5



void setProperty(char * key, char * value, int position);
char * getProperty(char * key);
int findInString(char * string, char  sign);
int tokenizer(struct map* token, char * stringToMap);

FILE *fp;
//array of properties
struct map properties[INITIAL_MAP_CAPACITY];




void loadPropertyFile(const char * filePath){
    fp=fopen(filePath, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: cant find property file\n");
        exit(1);
    }

    int charReaded=0;
    size_t len = 0;
    char * stringReaded;
    int readProperties=0;
    int i=0;
    struct map tempStruct;
    while((charReaded = getline(&stringReaded, &len, fp)) != -1){
        stringReaded[charReaded]='\0';
        if((i=tokenizer(&tempStruct, stringReaded))==-1){
            continue;
        }

        setProperty(tempStruct.key, tempStruct.value, readProperties);
        readProperties++;

    }


}


/*
 * getting property by key from property array
 */
char * getProperty(char * key){

    for(int i=0; i<INITIAL_MAP_CAPACITY; i++){
        if(strcmp(key, properties[i].key)==0){
            return properties[i].value;
        }
    }
    return NULL;
}

/*
 * Instead of const capacity in array i should use selfimplemted hashmap or linked list
 */
void setProperty(char * key, char * value, int position){

    strcpy(properties[position].key,key);
    strcpy(properties[position].value,value);

}

/*
 * find position first occurance of sign char in string
 */
int findInString(char * string, char  sign){

    int positionInString=0;
    char c;
    while((c=string[positionInString])!='\0'){

        if(c==sign){
            return positionInString;
        }
        positionInString++;
    }


    return -1;


}

/*
 * dividing read line from line on key=value structure
 */
int tokenizer(struct map* token, char * stringToMap){

    int position=findInString(stringToMap, '=');
    if(position==-1)
        return -1;

    strncpy(token->key, stringToMap, position);
    token->key[position]='\0';
    strncpy(token->value, stringToMap + position+1 , strlen(stringToMap)-position);

    return 0;
}