//
// Created by ilya2 on 9/4/2018.
//

#include "xmlAttribute.h"
#include <stdio.h>
#include <stdlib.h>

XmlAttr* attr_create(char *name, char *value){
    XmlAttr *res = malloc(sizeof(XmlAttr));
    initAttrNode(res);
    res->data->AttrName = name;
    res->data->AttrValue = value;
    return res;
}
void FreeAttr(XmlNode *attrNode){
    if((attrNode == 0)||(attrNode->type != ATTR)) {
        return;
    }
    XmlAttr *attr = (XmlAttr*) attrNode;
    if(attr->data) {
        if (attr->data->AttrName) {
            free(attr->data->AttrName);
        }
        if (attr->data->AttrValue) {
            free(attr->data->AttrValue);
        }
        free(attr->data);
    }
    freeNode(attrNode);
}
void printAttr(XmlNode *attrNode){
    XmlAttr* attr = (XmlAttr*) attrNode;
    printf("\t%s=\"%s\"", attr->data->AttrName, attr->data->AttrValue);
}
