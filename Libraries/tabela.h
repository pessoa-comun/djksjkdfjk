#ifndef TABELA_DE_SIMBOLOS

#define TABELA_DE_SIMBOLOS

int ehLetra(char c);
int carcaterRelevante(char c);
char *percorrerBuffer(char* buffer);
char *ehLabel(char* buffer);
void tabelaDeSimbolos(const char* arquivo);
int buscarTabela(char* label);
void destruirTabela();

#endif