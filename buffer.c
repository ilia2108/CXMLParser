#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "buffer.h"

void buffer_init(Buffer* res) {
   if(res) {
       memset(res, 0, sizeof(Buffer));
   }
}

void buffer_free(Buffer* res) {
    if(res) {
        if (res->text) {
            free(res->text);
        }
        memset(res, 0, sizeof(Buffer));
    }
}

void buffer_add(Buffer *res, char c){
    if(++res->length > res->capacity){
        res->capacity = res->length*2;
        res->text = realloc(res->text, res->capacity * sizeof(char));
        if(res->length==1){
            memset(res->text,0,res->capacity* sizeof(char));
        }
    }
    res->text[res->length-1] = c;
}

char* buffer_getstr(Buffer* buf){
    char* data = malloc((buf->length+1)* sizeof(char));
    data[buf->length]='\0';
    memcpy(data,buf->text,buf->length*sizeof(char));
    return data;
}

/*
int main(){
    Buffer b;
    buffer_init(&b);

    buffer_add(&b,'h');
    buffer_add(&b,'i');
    buffer_add(&b,'a');
    buffer_add(&b,'l');
    buffer_add(&b,'l');
    char* content = buffer_getstr(&b);

    printf("%s",content);
  //  system("pause");
  //getchar();
    buffer_free(&b);
    free(content);
}*/
