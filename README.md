# SCS_matheuristic

## Introdução  

Nesta pasta contém a implementação de dois algoritmos heurísticos que resolvem o problema da menor superstring comum onde um faz uso da metaheurística de algoritmo genético e outro da metaheurística simulated annealing.

- [Requisitos](#requisitos) - Requisitos de software necessarios para executar o projeto.
- [Compilação e execução](#compilação-e-execução) - Explicao das etapas de compilacao e execucao do projeto.
- [Autores](#autores) - Autores do projeto.

## Requisitos    

Compilador C++ 11 (__g++__).
	
	Obs.: Para usuários Linux, o g++ eh nativo, faltando apenas atualiza-lo para a versao 11.  
	Porém, caso deseje usar o g++ no Windows será necessário instalar-lo por meio do MinGW. 

## Compilação e execução

	Obs.: Todos os códigos digitados no terminal deverão ser na respectiva pasta raiz do programa.  

Execute na linha de terminal para a compilação e criação do objetos

```
$ make
```  
Ou, caso deseje apagar os objetos e os executáveis, digite  

```
$ make clean
```  
Em seguida, serão criados os seguintes arquivos binários (executáveis):

| Nome do executável: | Descrição: | 
| ---------- | ------------- |
|`genetic` 	| Algoritmo heurístico que usa algoritmo genético.  
|`simulatedAnnealing` 	|Algoritmo heurístico que usa simulated annealing.  
  
Diante disso, para executar o arquivo binário `genetic` basta:

```
$ ./bin/genetic
```
E, para executar o arquivo binário `simulatedAnnealing` basta: 

```
$ ./bin/simulatedAnnealing
```
  
## Autores 

| Name: | Github: | Email: |  
| ---------- | ------------- | ------------- |
|`Daniel Henrique` 	| https://github.com/Danhfg |_danhfg@ufrn.edu.br_  
|`Samuel Lucas` 	| https://github.com/Samuellucas97 |_samuellucas97@ufrn.edu.br_  


