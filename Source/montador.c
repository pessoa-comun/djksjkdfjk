#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "..\Libraries\macros.h"
#include "..\Libraries\tabela.h"


//Estrutura que contém todas as intrucões do conjunto de intrucões do myMIPS
struct {
    char instrucao[6];  //caracteres da instrução  => 5 caracteres + '\0'
    char binario[7];    //campo OPcode ou campo Funct (instrucao tipo R) => 6 caracteres + '\0'
}tabelaDeInstrucoes[] = {

    /* INSTRUCOES TIPO R*/
    { "ADD", "100000"},    //32
    { "SUB", "100010"},    //34
    { "AND", "100100"},    //36
    { "OR", "100101"},     //37
    { "XOR", "100110"},    //38
    { "JR", "001000"},     //8
    { "SLT", "101010"},    //42

    /* INSTRUCOES TIPO I*/
	{ "ADDI", "001000"},   //8
	{ "SW", "101011"},     //43
	{  "LW", "100011"},    //35
	{ "BEQ", "000100"},    //4
	{ "BNE", "000101"},    //5

    /* INSTRUCOES TIPO J*/
	{ "J", "000010" },     //2
	{ "JAL", "000011" }    //3
};

//Estrutura que contem 32 registradores
struct {
    char label[6];      // 5 caracteres + '\0'
    char endereco[6];   // 5 caracteres + '\0'

}bancoDeRegistradores[] = {
    { "$zero", "00000" },
	{ "$at", "00001" },
	{ "$v0", "00010" },
	{ "$v1", "00011" },
	{ "$a0", "00100" },
	{ "$a1", "00101" },
	{ "$a2", "00110" },
	{ "$a3", "00111" },
	{ "$t0", "01000" },
	{ "$t1", "01001" },
	{ "$t2", "01010" },
	{ "$t3", "01011" },
	{ "$t4", "01100" },
	{ "$t5", "01101" },
	{ "$t6", "01110" },
	{ "$t7", "01111" },
	{ "$s0", "10000" },
	{ "$s1", "10001" },
	{ "$s2", "10010" },
	{ "$s3", "10011" },
	{ "$s4", "10100" },
	{ "$s5", "10101" },
	{ "$s6", "10110" },
	{ "$s7", "10111" },
	{ "$t8", "11000" },
	{ "$t9", "11001" },
    { "$k0", "11010" },
	{ "$k1", "11011" },
    { "$gp", "11100" },
    { "$sp", "11101" },
    { "$fp", "11110" },
    { "$ra", "11111" },
};

/*Variáveis Globais*/
int PC = 0;             //Contador do programa
char OpcodeFunct[7];    //Binario do Opcode ou Funct
unsigned int nLinha;    //Contador da linha de programa

//Função de exceção que termina a excecução do programa
void exececao(FILE* entrada, FILE* saida, int num){
    fclose(entrada);        //Retira da memoria arquivo de entrada
    fclose(saida);          //Retira da memoria arquivo de saida
    
    // if(DELETAR_SAIDA_POR_ERROR)
    //     remove(arquivoSaida);   //Remove arquivo de saida

    exit(num);
}


//Função que recebe a instrucao decodificada em uma string e escreve o binário dela no arquivo de saida
void escreverBinario(char* instrucao, FILE* entrada, FILE* saida){

    //Inteiro de 8 bits (1 byte)
    uint8_t num;

    //Caso haja algum erro na string
    if(strlen(instrucao) != 32){
        puts("--- ERROR: INSTRUCION BIGGER THAN 32 BITS ---");
        exececao(entrada, saida, 3);
    }
    
    //Escreve a string de 32 carcateres em 4 inteiros de 8 bits na saida
    for(int i = 0; i < 4; i++){
        num = 0;
        for(int j = 0; j < 8; j++, instrucao++){
            int a;
            if(*(instrucao) == '1'){
                a = 2 <<  (6 - j);
                num += (a ? a : 1);
            }
        }

        //Funcao de escrita
        fwrite(&num, sizeof(uint8_t), 1, saida);
    }


}


//Função que identifica o tipo da instrucao myMIPS
int verificarTipo(char* OPcode){
    int i = 0;
    for(i = 0; i < 7; i++){
        if(!strcmpi(tabelaDeInstrucoes[i].instrucao, OPcode)){

            //Tipo R
            strcpy(OpcodeFunct, tabelaDeInstrucoes[i].binario);
            return 0;
            
        }
    }

    for(i = 7; i < 12; i++){
        if(!strcmpi(tabelaDeInstrucoes[i].instrucao, OPcode)){

            //Tipo I
            strcpy(OpcodeFunct, tabelaDeInstrucoes[i].binario);
            return 1;

        }
    }

    for(i = 12; i < 14; i++){
        if(!strcmpi(tabelaDeInstrucoes[i].instrucao, OPcode)){

            //Tipo J
            strcpy(OpcodeFunct, tabelaDeInstrucoes[i].binario);
            return 2;

        }
    }
    
    return -1;
}

//Função de tradução de instruções myMIPS do tipo R
void tipoR(FILE* entrada, FILE* saida, char* buffer){
    char regR[3][6]; /*Registradores RS RT RD, 5 bits + '\0' */
    char instrucao[33];

    //Intrução é Jump Register?
    if(!strcmpi(OpcodeFunct, tabelaDeInstrucoes[5].binario)){
        int l = TRUE;
        sscanf(buffer, "%*s %5[$zeroatvaskgpsfrZEROATVASKGPSFR0123456789]", regR[2]);


        //OPcode
        strcpy(instrucao, "000000");

        //Banco de Registradores tem 32 registradores
        for(int j = 0; j < 32; j++){
            if(!strcmpi(bancoDeRegistradores[j].label, regR[2])){
                strcat(instrucao, bancoDeRegistradores[j].endereco);
                l = FALSE;
                break;
            }
        }
        if(l){
            //Caso registrador não esteja no Banco de Registradores
            printf("--- ERROR IN LINE %d: REGISTER %s NOT FOUND ---\n", nLinha, regR[2]);
            exececao(entrada, saida, 2);
        }

        //RS e RT
        strcat(instrucao, "0000000000");

    } else {
        //Não é Jump Register

        //RD = regR[2], RS = regR[0] , RT = regR[1]
        sscanf(buffer, "%*s %5[$zeroatvaskgpsfrZEROATVASKGPSFR0123456789]%*c %5[$zeroatvaskgpsfrZEROATVASKGPSFR0123456789]%*c %5[$zeroatvaskgpsfrZEROATVASKGPSFR0123456789]", regR[2], regR[0], regR[1]);

        //Se for o registrador $zero como RD
        if((('z' == regR[2][1]) || ('Z' == regR[2][1]))){
            printf("--- ERROR IN LINE %d: REGISTER $zero MUST NOT BE WRITTEN ---\n", nLinha);
            exececao(entrada, saida, 2);
        } 

        //OPcode
        strcpy(instrucao, "000000");

        // Registadores
        // Na ordem: RS, RT, RD
        for(int i = 0; i < 3; i++){
            int l = FALSE;

            //Banco de Registradores tem 32 
            for(int j = 0; j < 32; j++){
                if(!strcmpi(bancoDeRegistradores[j].label, regR[i])){
                    l = TRUE;
                    strcat(instrucao, bancoDeRegistradores[j].endereco);
                    break;
                }
            }

            //Caso registrador não esteja no Banco de Registradores
            if(!l){
                printf("--- ERROR IN LINE %d: REGISTER %s NOT FOUND ---\n", nLinha, regR[i]);
                exececao(entrada, saida, 2);
            }
        }
    }

        //SHAMT
        strcat(instrucao, "00000");

        //FUNCT
        strcat(instrucao, OpcodeFunct);


    // Escrever no arquivo de saida
    // escreverArquivo(instrucao, entrada, saida);
    escreverBinario(instrucao, entrada, saida);
    
}

//Função de tradução de instruções myMIPS do tipo I
void tipoI(FILE* entrada, FILE* saida, char* buffer){
    char regI[2][6];    /* Registradores RS RT, 5 bits + '\0' */
    char Imm[17];       /* 16bits + '\0' */
    int desvio = FALSE;
    int LW = FALSE;
    int SW = FALSE;
    int ADDI = FALSE;
    int Label = FALSE;
    int numero;
    char instrucao[33];

    //Se eh um ADDI
    if(!strcmpi(tabelaDeInstrucoes[7].binario, OpcodeFunct))
        ADDI = TRUE;

    //Se eh um SW
    else if(!strcmpi(tabelaDeInstrucoes[8].binario, OpcodeFunct)) 
        SW = TRUE;

    //Se eh um LW
    else if(!strcmpi(tabelaDeInstrucoes[9].binario, OpcodeFunct))
        LW = TRUE;
    
    //Se eh um BNE ou BEQ
    else if(!strcmpi(tabelaDeInstrucoes[10].binario, OpcodeFunct) || !strcmpi(tabelaDeInstrucoes[11].binario, OpcodeFunct)) 
        desvio = TRUE;


    if(LW || SW){
        // INSTRUC RT, IMM(RS)
        //regI[0] == RS, regI[1] == RT
        sscanf(buffer, "%*s %5[$zeroatvaskgpsfrZEROATVASKGPSFR0123456789]%*c %16[^(](%5[$zeroatvaskgpsfrZEROATVASKGPSFR0123456789]%*c)", regI[0], Imm, regI[1]);
            
    } else {
        // INTS RS, RT, IMM
        //regI[0] == RS, regI[1] == RT
        sscanf(buffer, "%*s %5[$zeroatvaskgpsfrZEROATVASKGPSFR0123456789]%*c %5[$zeroatvaskgpsfrZEROATVASKGPSFR0123456789]%*c %16s", (ADDI ? regI[1]:regI[0]), (ADDI ? regI[0]:regI[1]), Imm);

    }

    //Se for ADDI, LW; o RT nao pode ser $zero
    if((LW || (!desvio && !SW)) && ((('z' == regI[1][1]) ||('Z' == regI[1][1])))){
        printf("--- ERROR IN LINE %d: REGISTER $zero MUST NOT BE WRITTEN ---\n", nLinha);
        exececao(entrada, saida, 2);
    }

    //Opcode
    strcpy(instrucao, OpcodeFunct);

    //Registradores
    // Na ordem: RS, RT
    for(int i = 0; i < 2; i++){
        int l = FALSE;
        //Banco de Registradores tem 32 
        for(int j = 0; j < 32; j++){
            if(!strcmpi(bancoDeRegistradores[j].label, regI[i])){
                l = TRUE;
                strcat(instrucao, bancoDeRegistradores[j].endereco);
                break;
            }
        }
        //Caso registrador não esteja no Banco de Registradores
        if(!l){
            printf("--- ERROR IN LINE %d: REGISTER %s NOT FOUND ---\n", nLinha, regI[i]);
            exececao(entrada, saida, 2);
        }
    }


    //Imm
    //Considerando que se nâo é uma letra, entao não pode ser um Label
    if(!ehLetra(Imm[0])){
        if(desvio){
            printf("--- ERROR IN LINE %d: %s IS NOT A VALID LABEL  ---\n", nLinha, Imm);
            exececao(entrada, saida, 2);
        } else {
            //Suponho que seja um numero de um ADDI
            for(int i = 1; Imm[i]; i++){
                
                //Nao pode aparecer uma letra no numero
                if(ehLetra(Imm[i])){
                    printf("--- ERROR IN LINE %d: %s IS NOT A VALID VALUE  ---\n", nLinha, Imm);
                    exececao(entrada, saida, 2);
                }
            }
        }
    } else {
        Label = TRUE;
        if(SW || LW){
            printf("--- ERROR IN LINE %d: %s IS NOT A VALID LABEL  ---\n", nLinha, Imm );
            exececao(entrada, saida, 2);
        }

        for(int i = 1; Imm[i]; i++){
            //So pode ser Label ser for apenas letras e numeros
            //eHLabel + eHnumeros
            if(!((ehLetra(Imm[i])) || (Imm[i] >= '0' && Imm[i] <= '9'))){
                printf("--- ERROR IN LINE %d: %s IS NOT A VALID LABEL  ---\n", nLinha, Imm );
                exececao(entrada, saida, 2);
            }
        }
    }

    //Caso seja um desvio e tenha label
    if(Label){
        if(desvio){
            numero = buscarTabela(Imm);

            if(numero < 0){
                printf("--- ERROR IN LINE %d: LABEL %s NOT FOUND IN SYMBOL TABLE ---\n", nLinha, Imm);
                exececao(entrada, saida, 2);
            }

            //Calculo do desvio de BEQ e BNE
            numero = (numero - PC)/4;
        
        } else {
            printf("--- ERROR IN LINE %d: %s IS NOT A VALID LABEL  ---\n", nLinha, Imm);
            exececao(entrada, saida, 2);
        }
    } else {
        numero = atoi(Imm);
    }


    //Immm (apenas positivo por hora)
    if(numero < 32768 && numero > -32768){
        //Codigo a seguir para converter para binario
        int binary[32] = {0};
        int negative = FALSE;
        int tam = 16;
        char string[17];

        if (!numero) {
            //Zero
            strcpy(string, "0000000000000000");
        } else {
            int k = 0;

            //Caso seja negativo
            if(numero < 0){
                negative = TRUE;
                numero = numero * -1;
            }

            //Algoritmo de Decimal pra Binario
            while (numero > 0) {
                binary[k] = numero & 1;
                numero = numero >> 1;
                k++;
            }


            if(!negative){
                for (int j = (tam-1), i = 0; j >= 0; j--, i++) {
                    string[i] = (binary[j]? '1':'0');


                    if(j == 0){
                        string[i+1] = '\0';
                    }
                    
                }


            } else {
                
                //Inverte bits
                for(int i = (tam-1); i >= 0; i--){
                    binary[i] = (binary[i] ? 0 : 1);
                }

                //Soma mais um bit
                binary[0] = (binary[0] ? 0 : 1);
                for(int i = 0; !binary[i++] && (i <= (tam-1));)
                    binary[i] = (binary[i] ? 0 : 1);
                    

                for (int j = (tam-1), i = 0; j >= 0; j--, i++) {
                    string[i] = (binary[j]? '1':'0');


                    if(j == 0){
                        string[i+1] = '\0';
                    }
                    
                    
                }
            }
    
        }

        strcat(instrucao, string);

        // escreverArquivo(instrucao, entrada, saida);
        escreverBinario(instrucao, entrada, saida);
        

    }else{
        printf("--- ERROR IN LINE %d: IMM FIELD IS OUT OF RANGE FOR 16 BITS %s---\n",nLinha, Imm );
        exececao(entrada, saida, 3);
    }


}


//Função de tradução de instruções myMIPS do tipo J
void tipoJ(FILE* entrada, FILE* saida, char* buffer){
    char addr[27]; /*26 bits + '\0'*/
    int numero; //Valor numero de addr
    char instrucao[32]; //Instrucao traduzida

    //INST ADDR
    sscanf(buffer, "%*s %26s", addr);

    //OpCode
    strcpy(instrucao, OpcodeFunct);

    //Addr
    if(!ehLetra(addr[0])){
        printf("--- ERROR IN LINE %d: %s IS NOT A VALID LABEL  ---\n", nLinha, addr);
        exececao(entrada, saida, 2);

    } else {

        for(int i = 1; addr[i]; i++){
            //So pode ser Label ser for apenas letras e nuemros
            //eHLabel + eHnumeros
            if(!((ehLetra(addr[i])) || (addr[i] >= '0' && addr[i] <= '9'))){
                printf("--- ERROR IN LINE %d: %s IS NOT A VALID LABEL  ---\n", nLinha, addr );
                exececao(entrada, saida, 2);
            }
        }
    }

    numero = buscarTabela(addr);
    if(numero < 0){
        printf("--- ERROR IN LINE %d: LABEL %s NOT FOUND IN SYMBOL TABLE ---\n", nLinha, addr);
        exececao(entrada, saida, 2);
    }
    numero += STARTBUFFER;

    if(numero < 33554431 && numero >= 0){
        //Codigo a seguir para converter para binario
        int binary[32] = {0};
        int tam = 26;
        char string[27];


        if (!numero) {
            //Zero
            strcpy(string, "00000000000000000000000000");
        } else {
            int k = 0;

            //Algoritmo de Decimal pra Binario
            while (numero > 0) {
                binary[k] = numero & 1;
                numero = numero >> 1;
                k++;
            }

            for (int j = (tam-1), i = 0; j >= 0; j--, i++) {
                string[i] = (binary[j]? '1':'0');
                
                if(j == 0){
                    string[i+1] = '\0';
                }
                
            }
        }   

        strcat(instrucao, string);
        
        // escreverArquivo(instrucao, entrada, saida);
        escreverBinario(instrucao, entrada, saida);


    }else{
        printf("--- ERROR IN LINE %d: ADDR FIELD IS OUT OF RANGE FOR 26 BITS ---\n", nLinha);
        exececao(entrada, saida, 3);
    }

}



//Função de Montagem que ler o arquivo de entrada, traduz para binário as instrucoes myMIPS e escreve no arquivo de saida
void montador(FILE* entrada, FILE* saida){
    char buffer[MAX]; //Buffer da linha de entrada (tamanho maximo definido por MAX)
    char OPcode[7]  /* 6 bits + '\0'*/;
    nLinha = 0; //Numero da linha lida


    //No Loop da montagem, o importante eh separar as instrucoes
    while(fgets(buffer, sizeof(buffer), entrada) != NULL){
        nLinha++;
        char *bufferRelevante, *restoDoBuffer;

        //Linha que eh somente um comentario pula
        if(buffer[0] == '#')
            continue;

        bufferRelevante = percorrerBuffer(&(buffer[0]));
        if(!bufferRelevante)  continue;

        restoDoBuffer = ehLabel(bufferRelevante);

        //Pode ser uma label com instrucao ou somente um label

        //Caso tenha label
        if(restoDoBuffer){
            char *p;
            
            p = percorrerBuffer(&(restoDoBuffer[0]));
            //Se depois da label nao tiver nada relevante
            
            if(!p) 

                continue;
            else{
                
                bufferRelevante = p;
            }
        }
        //Entra se supostamente for apenas uma instrucao
  
        PC += 4; // Incremento o PC como eh uma instrucao

        //Considero que o que tem no bufferRelevante eh uma instrucao
        sscanf(bufferRelevante, "%6s ", OPcode);
        switch (verificarTipo(OPcode)){
            case 0:
                tipoR(entrada, saida, bufferRelevante);
                break;
            case 1:
                tipoI(entrada, saida, bufferRelevante);
                break;
            case 2:
                tipoJ(entrada, saida, bufferRelevante);
                break;
            
            default: {
                
                printf("--- ERROR IN LINE %d: INSTRUCTION %s NOT FOUND ---\n", nLinha, OPcode);
                exececao(entrada, saida, 2);
                break;
            }
        }


    }
}