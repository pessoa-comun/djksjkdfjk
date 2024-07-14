#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "..\Libraries\macros.h"
#include "..\Libraries\tabela.h"
#include "..\Libraries\montador.h"

int ehArquivoASM(const char* arquivo){
    int i;

    for(i = 0; arquivo[i]; i++);

    return !strcmp(&(arquivo[i-4]), ".asm");

}



char* arquivoEntrada;
char* arquivoSaida;

int main(int argc, char** argv){
    //Variaveis
    char buffer[MAX];
    FILE *fpInput, *fpOutput;

    
    //Validacao de entrada
    if((argc > 3) || (argc < 2)){
        printf("--- ERROR: TOO %s ARGUMENTS AT COMMAND LINE ---\n", (argc > 3) ? "MANY":"FEW");
        return 2;

    }
    
    //Obtenção dos arquivos de entrada e saída
    arquivoEntrada = argv[1];
    if(argc > 2){
        // char temp[MAX];
        // strcpy(temp, argv[2]);
        // strcat(temp, ".dat");
        arquivoSaida =  argv[2];

    } else 
        arquivoSaida = "output.dat";
 
    
    //Validação do formato do arquivo de entrada
    if(!ehArquivoASM(arquivoEntrada)){
        puts("--- ERROR: INPUT FILE IS NOT IN ASSEMBLY LANGUAGE ---\n");
        return 2;
    }

    //Função que cria a tabela de símbolos do arquivo
    tabelaDeSimbolos(arquivoEntrada);


    //Manipular arquivo de entrada
    fpInput = fopen(arquivoEntrada, "r");
    if(!fpInput){
        printf("--- ERROR: INPUT FILE COULD NOT BE OPEN ---\n");
        return 1;
    }

    //Manipular arquivo de saída
    fpOutput = fopen(arquivoSaida, "wb");
    if(!fpOutput){
        printf("--- ERROR: OUTPUT FILE COULD NOT BE OPEN ---\n");
        return 1;
    }

    //Função de montagem do arquivo de saída
    montador(fpInput, fpOutput);


    //Fechamento dos arquivos
    fclose(fpInput);
    fclose(fpOutput);
    destruirTabela();

    // puts("\nFoi de boa");


}