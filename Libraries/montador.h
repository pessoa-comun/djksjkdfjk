#ifndef MONTADOR

#define MONTADOR

#include <stdio.h>

void escreverBinario(char* instrucao, FILE* entrada, FILE* saida);
void exececao(FILE* entrada, FILE* saida, int num);
int verificarTipo(char* OPcode);
void tipoR(FILE* entrada, FILE* saida, char* buffer);
void tipoI(FILE* entrada, FILE* saida, char* buffer);
void tipoJ(FILE* entrada, FILE* saida, char* buffer);
void montador(FILE* entrada, FILE* saida);

#endif