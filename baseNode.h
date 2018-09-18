//
// Created by ilya2 on 9/4/2018.
//

#ifndef XML_BASENODE_H
#define XML_BASENODE_H


#pragma pack(push)
#pragma pack(1)

#include <memory.h>
#include <stdio.h>

typedef enum _NodeType{
    TAG,
    ATTR,
    TEXT
} NodeType;


#define NODE_BASE  NodeType type;\
struct _Node* parent;\
struct _Node** children;\
int ChildCount;\
int childCapacity;\
void (*freeNode)(struct _Node*);

typedef struct _ATTRDATA{
    char *AttrName;
    char *AttrValue;
} XmlAttrData;

typedef struct _ATTR{
    NODE_BASE
    XmlAttrData* data;
} XmlAttr;

typedef struct _Node{
    NODE_BASE
    char* extension;
} XmlNode;

typedef struct _TAGDATA{
    char *TagName;
    int AttrCount;
    int AttrCapacity;
    XmlAttr **Attributes;
} XmlTagData;

typedef struct _TAG{
    NODE_BASE
    XmlTagData* data;
} XmlTag;

typedef struct _TEXTDATA{
    char *text;
    int length;
    int capacity;
} XmlTextData;

typedef struct _TEXT{
    NODE_BASE
    XmlTextData* data;
} XmlText;



void freeNode(struct _Node* node);

void freeText(struct _Node* textNode);
void initTextNode(XmlText* base);

//void freeAttr(struct _Node *attrNode);
void initAttrNode(XmlAttr *base);

//void freeTag(struct _Node *tagNode);
void initTagNode(XmlTag *base);

#pragma pack(pop)
#endif //XML_BASENODE_H
