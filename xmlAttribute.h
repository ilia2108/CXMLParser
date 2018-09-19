#ifndef XML_XMLATTRIBUTE_H
#define XML_XMLATTRIBUTE_H

#include "baseNode.h"

typedef struct _attribute {
    char *AttrName;
    char *AttrValue;
} XMLATTRIBUTE;

XmlAttr* attr_create(char *name, char *value);

void printAttr(XmlNode *attr);
void FreeAttr(XmlNode *attr);
#endif //XML_XMLATTRIBUTE_H
