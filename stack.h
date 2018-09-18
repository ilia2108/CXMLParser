//
// Created by ilya2 on 9/4/2018.
//

#ifndef XML_STACK_H
#define XML_STACK_H

#include "tag.h"
#include "baseNode.h"
typedef struct _stack
{
    XmlTag **data;
    int Length;
    int Capacity;

} Stack;

Stack* stack_create(void);
void stack_free(Stack *stack);
int push_back(Stack *stack, XmlTag *elem);
XmlTag * pop_back(Stack *stack);
//set stack->Top
XmlTag * peek(Stack* stack);

#endif //XML_STACK_H
