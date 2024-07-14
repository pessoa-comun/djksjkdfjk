// Estrutura de um label
#ifndef LISTA_ENCADEADA

#define LISTA_ENCADEADA

// Estrutura de um label
typedef struct 
{
    char nome[32];
    int pc;
}label;

// TAD LISTA ENCADIADA COM NO CABECALHO
typedef struct NODO
{
    char nome[32];
    int pc;
    struct NODO *prox;
}nodo;

typedef nodo * lista;

void criarLista(lista *L);
int listaVazia(lista L);
int tamLista(lista L);
void inserirLista(lista *L, label A);
void destruirLista(lista *L);
int buscaLista(lista *L, char* label);
void imprimirLista(lista L);

#endif