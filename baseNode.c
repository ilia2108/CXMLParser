//
// Created by ilya2 on 9/4/2018.
//

#include "baseNode.h"
#include <memory.h>
#include <stdlib.h>
#include "tag.h"
#include "xmlAttribute.h"

void freeNode(struct _Node* node){
    for(int i=0;i<node->ChildCount;i++){
        XmlNode* child = node->children[i];
        if(child->freeNode) {
            child->freeNode(child);
        }else{
            freeNode(child);
        }
    }
    if(node->children){
        free(node->children);
    }
    free(node);
}


//Text
void freeText(struct _Node* textNode){
    if((textNode==0)||( textNode->type!=TEXT)){
        return;
    }
    XmlText* text = (XmlText*)textNode;
    if(text->data){
        if(text->data->text)
            free(text->data->text);
        free(text->data);
    }
    freeNode(textNode);
}

void initTextNode(XmlText* base){
    memset(base,0,sizeof(XmlText));
    base->type=TEXT;
    base->data=malloc(sizeof(XmlTextData));
    memset(base->data,0,sizeof(XmlTextData));
    base->freeNode=&freeText;
}


//Atribute

void initAttrNode(XmlAttr *base){
    memset(base, 0, sizeof(XmlAttr));
    base->type = ATTR;
    base->data = malloc(sizeof(XmlAttrData));
    memset(base->data, 0, sizeof(XmlAttrData));
    base->freeNode = &FreeAttr;
}

//Tag
void initTagNode(XmlTag *base){
    memset(base, 0, sizeof(XmlTag));
    base->type = TAG;
    base->data = malloc(sizeof(XmlTagData));
    memset(base->data, 0, sizeof(XmlTagData));
    base->freeNode = &FreeTag;
}