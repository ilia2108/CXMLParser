
#ifndef XML_BUFFER_H
#define XML_BUFFER_H


typedef struct _buffer
{
    char *text;
    int length;
    int capacity;
} Buffer;

void buffer_init(Buffer* res);
void buffer_free(Buffer *res);
void buffer_add(Buffer *res, char c);
char* buffer_getstr(Buffer* buf);


#endif //XML_BUFFER_H
