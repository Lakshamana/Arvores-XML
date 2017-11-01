#include <stdlib.h>
#include <string.h>

typedef struct tag{
    char name[10];
    struct tag *prox;
}Tag;

/*
*   void push(Tag **ptr, char *nome);
*   void pop(Tag **ptr, char *nome);
*   int ler_tag(FILE **file, Tag **ptr); //Retornar 0 caso nomes de tags comecem por " ", "\", "`", "´".
*       //não pode haver espaços na tag.
*   int valida_tag_close(Tag *ptr, char *nome);
*   int empty_stack(Tag *ptr); //Verifica se a pilha de tags está vazia.
*   void show_tags(Tag *ptr);
*/

void push(Tag **ptr, char *nome){
    Tag *p1 = *ptr;
    p1 = (Tag *)malloc(sizeof(Tag));
    strcpy(p1->name, nome);
    if(*ptr == NULL)
        p1->prox = NULL;
    else
        p1->prox = *ptr;
    *ptr = p1;
}

int empty_stack(Tag *ptr){
    return (ptr == NULL);
}

void show_tags(Tag *ptr){
    while(ptr != NULL){
        printf("%s", &ptr->name);
        ptr = ptr->prox;
    }
}
