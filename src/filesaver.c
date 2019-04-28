//
// Created by xbox3 on 25.04.2019.
//

#include "filesaver.h"

void deepCopyString(char * structField,const char * string);

/*
 * received text is in json format
 * used this function to parse json object to messageStruct type structure
 */
void convertJsonToStruct(char* json, struct messageStruct * structdata_to_save){

    struct json_object *parsed_json;
    struct json_object *broadcasterID;
    struct json_object *data;

    parsed_json = json_tokener_parse(json);
    json_object_object_get_ex(parsed_json, "broadcasterID", &broadcasterID);
    json_object_object_get_ex(parsed_json, "data", &data);
    const char * bdcID=json_object_get_string(broadcasterID);
    const char * dataToSave=json_object_get_string(data);
    if(bdcID!=NULL&&dataToSave!=NULL) {
        deepCopyString(structdata_to_save->broadcasterID, bdcID);
        deepCopyString(structdata_to_save->data, dataToSave);
    }

}

/*
 * save converted json to file
 */
void saveStructToFile(struct messageStruct *structDataToSave, FILE *fp){
    fprintf (fp, "%s: ",structDataToSave->broadcasterID);
    fprintf (fp, "%s\n",structDataToSave->data);

}

/*
 * save json to file
 */
int saveStringToFile(char * message, FILE *fp){
    int bytesSaved=fprintf (fp, "%s: ",message);
    return bytesSaved;
}

/*
 * strcpy function
 */
void deepCopyString(char * structField,const char * string){
    char c;
    int i=0;
    while((c=string[i])!='\0'){
        structField[i]=c;
//	printf("%c", c);
        i++;
    }
//
//    printf("\n");
    structField[i]='\0';

}
