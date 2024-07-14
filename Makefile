# -*- Makefile -*-

NAME = Assembler
SRC = ./Source
LIB = ./Libraries
OBJ = ./Objects

all: $(OBJ)/main.o $(OBJ)/tabela.o $(OBJ)/lista.o $(OBJ)/montador.o
	gcc $(OBJ)/*.o -o $(NAME)

$(OBJ)/main.o: $(SRC)/main.c
	gcc -c $(SRC)/main.c -I LIB -o $(OBJ)/main.o

$(OBJ)/tabela.o: $(SRC)/tabela.c
	gcc -c $(SRC)/tabela.c -I LIB -o $(OBJ)/tabela.o

$(OBJ)/lista.o: $(SRC)/lista.c
	gcc -c $(SRC)/lista.c -I LIB -o $(OBJ)/lista.o
	
$(OBJ)/montador.o: $(SRC)/montador.c
	gcc -c $(SRC)/montador.c -I LIB -o $(OBJ)/montador.o

clear:
	rm -rf $(OBJ)/*


