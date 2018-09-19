#include "tag.h"
#include "xmlAttribute.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "stack.h"

int IsUniqueAttr(XmlTag *tag, char *name){
    if(tag->children != NULL) {
        for (int i = 0; i < tag->ChildCount; i++) {
            if (tag->children[i]->type == ATTR) {
                if (((XmlAttr *) tag->children[i])->data->AttrName == name)
                    return 0;
            }
        }
    }
    return 1;
}

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
    if(IsUniqueAttr(tag, attribute->data->AttrName)) {
        tag->ChildCount++;
        if (tag->ChildCount > tag->childCapacity) {
            tag->childCapacity = tag->ChildCount * 2;
            tag->children = realloc(tag->children, tag->childCapacity * sizeof(XmlNode **));
            if (!tag->children) {
                return 0;
            }
        }
        tag->children[tag->ChildCount - 1] = (XmlNode *) attribute;
        attribute->parent = (XmlNode *) tag;
        return tag->ChildCount;
    }
    else return 0;
}

int tag_remove_attr_byref(XmlTag* tag,XmlAttr* attribute){
    for(int i = 0; i< tag->ChildCount; ++i){
        if(tag->children[i]->type == ATTR){
            if((XmlAttr*)tag->children[i] == attribute) {
                FreeAttr((XmlNode *) tag->children[i]);
                for (int j = i; j < tag->ChildCount - 1; ++j)
                    if(tag->children[j]->type == ATTR)
                        tag->children[j] = tag->children[j + 1];
                return --tag->ChildCount;
            }
        }
    }
    return tag->ChildCount;
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
    for(int i = 0; i< tag->ChildCount; ++i){
        if(tag->children[i]->type == ATTR){
            if(((XmlAttr*)tag->children[i])->data->AttrName==name) {
                FreeAttr((XmlNode*)tag->children[i]);
                for (int j = i; j < tag->ChildCount - 1; ++j)
                    if(tag->children[i]->type == ATTR)
                        tag->children[j] = tag->children[j + 1];
                return --tag->ChildCount;
            }
        }
    }
    return tag->ChildCount;
}
XmlAttr* tag_find_attr_byname(XmlTag *tag, char *name) {
    for(int i = 0; i< tag->ChildCount; ++i){
        if(tag->children[i]->type==ATTR)
            if(((XmlAttr*)tag->children[i])->data->AttrName==name)
                return (XmlAttr*)tag->children[i];
    }
    return 0;
}

XmlAttr* tag_find_attr_byref(XmlTag *tag, XmlAttr *attribute){
    for(int i = 0; i< tag->ChildCount; i++){
        if(tag->children[i]->type == ATTR){
            if((XmlAttr*)tag->children[i] == attribute)
                return (XmlAttr*)tag->children[i];
        }
    }
    return NULL;
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
        free(tag->data);
    }
    freeNode(tagNode);
}
void printTag(XmlTag *tag, int isFirst) {
    int flag = 0;
    printf("<%s", tag->data->TagName);
    if(isFirst){
        isFirst = 0;
        printf(">\n");
    }
    for(int i = 0; i< tag->ChildCount;i++){
        if(tag->children[i]->type==ATTR){
            printf("%c",' ');
            printAttr(tag->children[i]);
        }
        else if(tag->children[i]->type == TEXT){
            printf(">%s", ((XmlText*)tag->children[i])->data->text);
            flag = 1;
        }
        else if(tag->children[i]->type == TAG){
            printf("%c",'\r');
            printTag((XmlTag*)tag->children[i], isFirst);
            flag = 1;
        }
    }

    if(flag){
        printf("</%s>\n", tag->data->TagName);
    }
    else
        printf("/>\n");

}
XmlText* tag_text_create(char *text){
    XmlText* res = malloc(sizeof(XmlText));
    initTextNode(res);
    res->data->text = text;
    return res;
}
int tag_add_text(XmlTag *tag, XmlText *text){
    if(!tag->data->HasText) {
        tag->ChildCount++;
        if (tag->ChildCount > tag->childCapacity) {
            tag->childCapacity = tag->ChildCount * 2;
            tag->children = realloc(tag->children, tag->childCapacity * sizeof(XmlNode **));
            if (!tag->children) {
                return 0;
            }
        }
        tag->children[tag->ChildCount - 1] = (XmlNode *) text;
        text->parent=(XmlNode*)tag;
        tag->data->HasText = 1;
        return tag->ChildCount;
    }
    else return 0;
}