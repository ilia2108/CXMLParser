#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tag.h"
#include "xmlAttribute.h"
#include "buffer.h"
#include "stack.h"
#include <ctype.h>


typedef enum _States {
    Start,
    TagOpen,
    TagName,
    AttrName,
    AttrValueStart,
    AttrValue,
    AttrValueEnd,
    Text,
    TagClose,
    EndTagStart,
    EndTagName,
    EndTagEnd
} State;

bool onlyLetters(Buffer *buffer);

void addCharToName(Buffer *buffer, char c){
    if(isalpha(c))
        buffer_add(buffer, c);
}

int clearLastStackElem(Buffer *buffer, Stack *stack){
    if((buffer->text == NULL) ||((peek(stack)!=NULL) && !strncmp(peek(stack)->data->TagName,buffer->text,buffer->length))) {
        pop_back(stack);
        buffer_free(buffer);
        return 0;
    }
    return  -1;
}
XmlTag* Parse(){
    int ErrorState = 0;
    State CurrentState = Start;
    int c;
    Buffer buffer;
    buffer_init(&buffer);
    XmlTag* tmpTag = NULL;
    XmlTag* root = NULL;
    int isSecondQuote = 0;
    Stack *stack = stack_create();
    char *attrName= NULL, *attrValue= NULL;
    XmlAttr *tmpAttr= NULL;
    FILE *input = fopen("/Users/ilia_2108/Desktop/XML/input.xml", "r");
    int lineCount = 1;
    int charCount = 0;
    XmlText *text = NULL;
    while((c = getc(input)) != EOF) {
        if(c == '\n'){
            lineCount++;
            charCount = 0;
        }
        else charCount++;
        switch (c) {
            case '<':
                switch (CurrentState){
                    case Start:
                    case TagClose:
                    case EndTagEnd:
                        CurrentState = TagOpen;
                        break;
                    case Text:
                        if(onlyLetters(&buffer)) {
                            text = tag_text_create(buffer_getstr(&buffer));
                            if(!tag_add_text(tmpTag, text)){
                                ErrorState = -1;
                                goto end;
                            }
                            buffer_free(&buffer);
                            CurrentState = TagOpen;
                        }
                        else {
                            ErrorState = -1;
                            goto end;
                        }
                        break;
                    default:
                        ErrorState = -1;
                        goto end;
                }
                break;
            case ' ':
                switch (CurrentState){
                    case TagOpen:
                        CurrentState = TagName;
                        break;
                    case TagName:
                        tmpTag = tag_create(buffer_getstr(&buffer), peek(stack));
                        push_back(stack, tmpTag);
                        buffer_free(&buffer);
                        CurrentState = AttrName;
                        break;
                    case AttrName:
                        if(!attrName) {
                            attrName = buffer_getstr(&buffer);
                            buffer_free(&buffer);
                        }
                        break;
                    case AttrValueEnd:
                    case Start:
                        break;
                    case AttrValue:
                    case Text:
                        buffer_add(&buffer, c);
                        break;
                    case TagClose:
                    case EndTagEnd:
                    case AttrValueStart:
                        buffer_free(&buffer);
                        break;
                    default:
                        ErrorState = -1;
                        goto end;
                }
                break;
            case '\"':
                if(!isSecondQuote) {
                    isSecondQuote++;
                    //CurrentState = AttrValue;
                }else{
                    CurrentState = AttrValueEnd;
                    attrValue = buffer_getstr(&buffer);
                    tmpAttr = attr_create(attrName, attrValue);
                    attrName=NULL;
                    attrValue=NULL;
                    tag_add_attr(tmpTag, tmpAttr);
                    buffer_free(&buffer);
                    isSecondQuote = 0;
                }
                break;
            case '/':
                if(CurrentState == TagOpen)
                    CurrentState = EndTagStart;
                else if ((CurrentState == AttrValueEnd) || (CurrentState == TagName))
                    CurrentState = EndTagEnd;
                break;
            case '>':
                switch (CurrentState){
                    case TagName:
                        tmpTag = tag_create(buffer_getstr(&buffer), peek(stack));
                        if(!root){
                            root = tmpTag;
                        }
                        push_back(stack, tmpTag);
                        buffer_free(&buffer);
                        CurrentState = TagClose;
                        break;
                    case AttrValueEnd:
                        if(attrName&&attrValue) {
                            tmpAttr = attr_create(attrName, attrValue);
                            tag_add_attr(tmpTag, tmpAttr);
                        }
                        attrName=NULL;
                        attrValue=NULL;
                        CurrentState = TagClose;
                        break;
                    case EndTagName:
                        CurrentState = EndTagEnd;
                        if((ErrorState = clearLastStackElem(&buffer, stack)) == -1){
                            goto end;
                        }
                        break;
                    case EndTagEnd:
                        CurrentState = Start;
                        if((ErrorState = clearLastStackElem(&buffer, stack)) == -1){
                            goto end;
                        }
                        break;
                    default:
                        ErrorState = -1;
                        goto end;
                }
                break;

            case '\n':
                switch(CurrentState){
                    case TagName:
                    case AttrName:
                    case AttrValue:
                        ErrorState = -1;
                        goto end;
                    default:
                        break;
                }
                break;
            case '=':
                switch(CurrentState){
                    case Text:
                    case AttrValue:
                        buffer_add(&buffer, (char) c);
                        break;
                    case AttrName:
                         CurrentState = AttrValueStart;
                         attrName=buffer_getstr(&buffer);
                         buffer_free(&buffer);
                        break;
                    default:
                        break;
                }
                break;
            default:
                switch (CurrentState){
                    case TagOpen:
                        CurrentState = TagName;
                        addCharToName(&buffer, (char)c);
                        break;
                    case EndTagStart:
                        CurrentState = EndTagName;
                        addCharToName(&buffer, (char) c);
                        break;
                    case AttrValueStart:
                        CurrentState = AttrValue;
                        buffer_add(&buffer, (char) c);
                        break;
                    case TagClose:
                        CurrentState = Text;
                        buffer_add(&buffer, (char) c);
                        break;
                    case Text:
                        buffer_add(&buffer, (char) c);
                        break;
                    case EndTagName:
                    case TagName:
                    case AttrName:
                        if(isalpha(c)) {

                            buffer_add(&buffer, (char) c);
                            break;
                        }
                    case AttrValue:
                        buffer_add(&buffer, (char) c);
                        break;
                    case AttrValueEnd:
                        CurrentState = AttrName;
                        buffer_add(&buffer, (char) c);
                        break;
                    default:
                        ErrorState = -1;
                        goto end;
                }
                break;
        }

    }
    if(stack->Length){
        ErrorState = -1;
    }
    end:
    if(attrName){
        free(attrName);
    }
    if(attrValue){
        free(attrValue);
    }
    fclose(input);
    if(ErrorState != -1) {
        printTag(stdout, root, 1);
    }
    else {
        printf("%d, %d", lineCount, charCount);
    }
    buffer_free(&buffer);
    //free(buffer);
    stack_free(stack);
    return root;

}




bool onlyLetters(Buffer *buffer) {
    for(int i = 0; i < buffer->length; i++){
        if(buffer->text[i] == '\"' || buffer->text[i] == '>' || buffer->text[i] == '<')
            return false;
    }
    return true;
}

int main() {
    XmlTag* tag = Parse();
    FILE *file = fopen("out.xml", "w");
    printTag(file, tag, 1);
    fflush(file);
    fclose(file);
    FreeTag(tag);



    return 0;
}