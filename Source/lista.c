#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Estrutura de um label
typedef struct 
{
    char nome[32];
    int pc;
}label;

// TAD LISTA ENCADIADA COM NO CABECALHO (sem retirada de elemento)
typedef struct NODO
{
    char nome[32];
    int pc;
    struct NODO *prox;
}nodo;

typedef nodo * lista;

void criarLista(lista *L){
    (*L) = (nodo *) malloc(sizeof(nodo));
    if(!(*L)){
        puts("ERRO");
        exit(0);
    }

    (*L)->pc = 0;
    (*L)->prox = NULL;
    
}

int listaVazia(lista L){
    return (L->pc == 0);
}

int tamLista(lista L){
    return (L->pc);
}

void inserirLista(lista *L, label A){
    
    lista p; 
    nodo* novo = (nodo *) malloc(sizeof(nodo));
    if(!novo){
        puts("ERRO");
        exit(0);
    }

    strcpy(novo->nome, A.nome);
    novo->pc = A.pc;
    novo->prox = NULL;
    (*L)->pc = (*L)->pc + 1;
    for(p = (*L); p->prox; p = p->prox);

    p->prox = novo;

}

void destruirLista(lista *L){
    nodo* p;
    while(*L){
        p = *L;
        (*L) = (*L)->prox;
        free(p);
    }
}

int buscaLista(lista *L, char* label){
    nodo *p = ((*L)->prox);
    int i = 0;
    int limit = (*L)->pc;

    //Buscar na lista pelo nome do Label
    while(p && (i++ < limit)){
        if(!strcmpi(label, p->nome)){
            //Retorna o pc do label encontrado
            return p->pc;
        } else {
            p = p->prox;
        }
    }

    //Se label nao estar na tabela
    return -1;

}

void imprimirLista(lista L){
    nodo *p;
    L = L->prox;
    while(L){
        p = L;
        L = L->prox;
        printf("nome: [%s]\n", p->nome);
        printf("pc: [%d]\n", p->pc);
        printf("----------------------\n");
    }
}