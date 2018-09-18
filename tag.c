#include "tag.h"
#include "xmlAttribute.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

XmlTag* tag_create(char *tagName, XmlTag* parent) {
    XmlTag* res = malloc(sizeof(XmlTag));
    initTagNode(res);
    res->data->TagName = tagName;
    if(parent){
        tag_add_child(parent,res);
    }
    return res;
    //attrCapacity = childCapacity = 0;
}
int tag_add_attr(XmlTag* tag,XmlAttr* attribute){
    tag->ChildCount++;

    if(tag->ChildCount > tag->childCapacity) {
        tag->childCapacity = tag->ChildCount*2;
        tag->children=realloc(tag->children, tag->childCapacity* sizeof(XmlNode**));
        if(!tag->children){
            return false;
        }
    }
    tag->children[tag->ChildCount-1] = attribute;
    return tag->ChildCount;
}
int tag_remove_attr_byref(XmlTag* tag,XmlAttr* attribute){
    for(int i = 0; i< tag->data->AttrCount; ++i){
        if(tag->data->Attributes[i] == attribute){
            FreeAttr((XmlNode*)tag->data->Attributes[i]);
            for(int j = i; j<tag->data->AttrCount - 1; ++j)
                tag->data->Attributes[j] = tag->data->Attributes[j+1];
            return --tag->data->AttrCount;
        }
    }
    return tag->data->AttrCount;
}
int tag_add_child(XmlTag* tag,XmlTag* child){
    tag->ChildCount++;
    if(tag->ChildCount > tag->childCapacity ) {
        tag->childCapacity = tag->ChildCount*2;
        tag->children = realloc(tag->children, tag->childCapacity* sizeof(XmlTag**));
        if(tag->children == NULL)
            return false;
    }
    tag->children[tag->ChildCount - 1] = (XmlNode*)child;
    child->parent = (XmlNode*)tag;
    return tag->ChildCount;
}
int tag_remove_attr_byname(XmlTag* tag,char *name) {
    for(int i = 0; i< tag->data->AttrCount; ++i){
        if(tag->data->Attributes[i]->data->AttrName==name) {
            FreeAttr((XmlNode*)tag->data->Attributes[i]);
            for (int j = i; j < tag->data->AttrCount - 1; ++j)
                tag->data->Attributes[j] = tag->data->Attributes[j + 1];
            return --tag->data->AttrCount;
        }
    }
    return tag->data->AttrCount;
}
XmlAttr* tag_find_attr_byname(XmlTag *tag, char *name) {
    for(int i = 0; i< tag->data->AttrCount; ++i){
        if(tag->data->Attributes[i]->data->AttrName==name)
            return tag->data->Attributes[i];
    }
    return false;
}
bool tag_find_tag(XmlTag *parent, XmlTag *child){
    for(int i = 0; i < parent->ChildCount; ++i) {
        if((XmlTag*)parent->children[i] == child)
            return true;
    }
    return false;
}
int tag_remove_childtag(XmlTag *parent, XmlTag *child){
    for(int i = 0; i < parent->ChildCount; ++i) {
        if((XmlTag*)parent->children[i] == child) {
            FreeTag((XmlNode*)child);
            for (int j = i; j < parent->ChildCount - 1; ++j)
                parent->children[j] = parent->children[j + 1];
            return --parent->ChildCount;
        }
    }
    return parent->ChildCount;
}
void FreeTag(XmlNode* tagNode) {
    if((tagNode == 0)||(tagNode->type != TAG)){
        return;
    }
    XmlTag *tag = (XmlTag*) tagNode;
    if(tag->data){
        if(tag->data->TagName){
            free(tag->data->TagName);

        }
        if(tag->data->Attributes){
            for(int i = 0;i< tag->data->AttrCount;i++) {
                FreeAttr((XmlNode*)tag->data->Attributes[i]);
            }
            free(tag->data->Attributes);
        }
        free(tag->data);
    }
    freeNode(tagNode);
}
void printTag(XmlTag *tag){
    printf("%s\n", tag->data->TagName);
    for(int i = 0; i< tag->ChildCount;i++){
        if(tag->children[i]->type==ATTR){
            printf("%c", '\t');
            printAttr(tag->children[i]);
        }
        else if(tag->children[i]->type == TAG){
            printf("%c", '\t');
            printTag((XmlTag*)tag->children[i]);
        }
    }

}