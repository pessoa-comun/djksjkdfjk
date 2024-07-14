#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "..\Libraries\macros.h"
#include "..\Libraries\lista.h"


//Função que identifica uma letra
int ehLetra(char c){
    return (( c >= 'A') && (c <= 'Z') ) || ( (c >= 'a') && (c <= 'z') );
}

//Função que identifica um caracter relevante
int carcaterRelevante(char c){

    //ascii code 32 eh o caracter do espaco
    return ((c != 32) && (c != '#') && (c != '\n') && (c != '\r') && (c != '\t'));
}


//Função que percorre o buffer procurando algum caracter relevante
char *percorrerBuffer(char* buffer){
    
    for(; *(buffer); buffer++){

        if(carcaterRelevante(*buffer))
            return buffer;

        if(*buffer == '#')
            return NULL;
    }

    return NULL;
}


//Função que identifica se eh um label
char *ehLabel(char* buffer){ 
    int i = 0;
    for(; *(buffer); buffer++){
        
        //Caso ':' seja o primeiro caracter
        if(!i && (*buffer == ':')){
            puts("ERRO");
            exit(0);
        } else {
            i++;
        }

        //Se tem ':' eh um Label
        if(*buffer == ':'){
            
            //Temina o Label no ':'
            *buffer = '\0';

            //Retornar ponteiro depois do fim do Label
            return (++buffer);
        }

        if(*buffer == '#')
            return NULL;
    }

    //Se nao for um label, eu suponho que seja uma instrucao
    return NULL;
}


lista tabela = NULL;    //Lista encadiada que serve como tabela de simbolos

//Função desalocar a tabela
void destruirTabela(){
    destruirLista(&tabela);
}

//Função de busca sequencial de um label na tabela
int buscarTabela(char* label){
    return buscaLista(&tabela, label);
}

//Funcao que cria uma lista encadiada com os Labels do arquivo, relacionando cada label com o valor do PC na sua determinada linha
void tabelaDeSimbolos(const char* arquivo){
    char buffer[MAX];
    unsigned int pc = 0;
    int n = 0;
    FILE* fp;

    criarLista(&tabela);

    fp = fopen(arquivo, "r");
    
    while(fgets(buffer, sizeof(buffer), fp) != NULL){
        char *bufferRelevante, *restoDoBuffer;

        //Linha so eh comentario
        if(buffer[0] == '#')
            continue;


        bufferRelevante = percorrerBuffer(&(buffer[0]));
        if(!bufferRelevante)  continue;


        //Esse caracter pode ser de um lebel ou uma instrucao
        //Label obrigatoriamente termina com ':'

        //Se bufferRelevante tiver um label, restoDoBuffer vai ter o que esta depois do label
        //Caso nao for um label, restoDoBuffer tem NULL
        restoDoBuffer = ehLabel(bufferRelevante);

        //Entra se for um Label
        if(restoDoBuffer){
            char *p;
            label A;
            //restoDoBuffer ou eh uma instrucao ou um comentario
            //Se for uma instrucao, apenas incrementa o pc
            //Se for um comentario ou nada de relevante, pula de linha
            

            //Poderia tentar verificar se eh uma instrucao ou nao, mas agora nao precisa
            p = percorrerBuffer(&(restoDoBuffer[0]));
            


            //Colocar label encontrado dentro da lista
            //Bufferrelevante tem o nome do label
            strcpy(A.nome, bufferRelevante);
            //Pc atual
            A.pc = pc;
            inserirLista(&tabela, A);
            
            //Como nao eh uma instrucao, entao pula de linha
            if(!p) continue;

        }

        //Como eh uma instrucao, incremanta o pc
        pc += 4;

    }
}
