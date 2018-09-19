

#ifndef XML_TAG_H
#define XML_TAG_H

#include "xmlAttribute.h"
#include <stdbool.h>
#include "baseNode.h"

XmlTag* tag_create(char *tagName, XmlTag* parent);
int tag_add_attr(XmlTag* tag,XmlAttr* attribute);
int tag_remove_attr_byref(XmlTag* tag,XmlAttr* attribute);
int tag_remove_attr_byname(XmlTag* tag,char *name);
XmlAttr* tag_find_attr_byref(XmlTag *tag, XmlAttr *attribute);
XmlAttr* tag_find_attr_byname(XmlTag *tag, char *name);
int tag_add_child(XmlTag* tag,XmlTag* child);
bool tag_find_tag(XmlTag *parent, XmlTag *child);
int tag_remove_childtag(XmlTag *parent, XmlTag *child);
void FreeTag(XmlNode* tagNode);
void printTag(XmlTag *tag, int isFirst);
XmlText* tag_text_create(char *text);
int tag_add_text(XmlTag *tag, XmlText *text);
int IsUniqueAttr(XmlTag *tag, char *name);
#endif //XML_TAG_H