RM = rm -rf
#Compilador
CC=g++

#Variaveis para os subdiretorios
#LIB_DIR=./lib Não haverá pois nenhuma biblioteca será usada
INC_DIR=./include
SRC_DIR=./src
OBJ_DIR=./build
BIN_DIR=./bin

#Opcoes de compilacao 
CFLAGS=	-pedantic -ansi -std=c++11 

#Garante que os alvos desta lista não sejam confundidos com arquivos de mesmo nome 
.PHONY:	all	clean	debug	

#Ao final da compilacão, remove os arquivos objetos
all: init genetic simulatedAnnealing genInstances

debug:	CFLAGS += -g -O0
debug:	genetic simulatedAnnealing

# Cria a pasta/diretório bin e a obj
init:
	@mkdir -p $(OBJ_DIR)/
	@mkdir -p $(BIN_DIR)

# Alvo (target) para a construcao do executavel questao01
# Define os arquivos genetic.o, calcula.o, perimetro.o, volume.o e main.o como dependencias
genetic: CFLAGS+= -I$(INC_DIR)/
genetic: $(OBJ_DIR)/genetic.o 
	@echo "============="
	@echo "Ligando o alvo $@"
	@echo "============="
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $^
	@echo "+++ [Executavel genetic criado em $(BIN_DIR)] +++"
	@echo "============="

# Alvo (target) para a construcao do objeto genetic.o
# Define os arquivos genetic.cpp e genetic.h como dependencias.
$(OBJ_DIR)/genetic.o: $(SRC_DIR)/genetic/ga.cpp
	$(CC) -c $(CFLAGS) -o $@ $<


# Alvo (target) para a construcao do executavel questao01
# Define os arquivos genetic.o, calcula.o, perimetro.o, volume.o e main.o como dependencias
simulatedAnnealing: CFLAGS+= -I$(INC_DIR)/
simulatedAnnealing: $(OBJ_DIR)/simulatedAnnealing.o 
	@echo "============="
	@echo "Ligando o alvo $@"
	@echo "============="
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $^
	@echo "+++ [Executavel simulatedAnnealing criado em $(BIN_DIR)] +++"
	@echo "============="

# Alvo (target) para a construcao do objeto genetic.o
# Define os arquivos genetic.cpp e genetic.h como dependencias.
$(OBJ_DIR)/simulatedAnnealing.o: $(SRC_DIR)/simulatedAnnealing/sa.cpp
	$(CC) -c $(CFLAGS) -o $@ $<

# Alvo (target) para a construcao do executavel questao01
# Define os arquivos genetic.o, calcula.o, perimetro.o, volume.o e main.o como dependencias
genInstances: CFLAGS+= -I$(INC_DIR)/
genInstances: $(OBJ_DIR)/genInstances.o 
	@echo "============="
	@echo "Ligando o alvo $@"
	@echo "============="
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $^
	@echo "+++ [Executavel genInstances criado em $(BIN_DIR)] +++"
	@echo "============="

# Alvo (target) para a construcao do objeto genetic.o
# Define os arquivos genetic.cpp e genetic.h como dependencias.
$(OBJ_DIR)/genInstances.o: $(SRC_DIR)/genInstances.cpp
	$(CC) -c $(CFLAGS) -o $@ $<
#removendo os .o e os binários
clean:
	$(RM) $(BIN_DIR)/*
	$(RM) $(OBJ_DIR)/*

#FIM DO MAKEFILE
