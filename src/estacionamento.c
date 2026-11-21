#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "estacionamento.h"

void inicializar_mapa(int mapa[FILEIRAS][COLUNAS]){

    for(int i = 0; i < FILEIRAS; i++){
        for(int j = 0; j < COLUNAS; j++){
            mapa[i][j] = VAGA_LIVRE;
        }
    }
}

void exibir_mapa(int mapa[FILEIRAS][COLUNAS]){

    printf("\n      ");
    for(int j = 0; j < COLUNAS; j++){
        printf("C%d ", j + 1);
    }
    printf("\n");

    for(int i = 0; i < FILEIRAS; i++){
        printf(" F%d   ", i + 1);
        for(int j = 0; j < COLUNAS; j++){
            printf("%c  ", mapa[i][j] == VAGA_OCUPADA ? 'X' : '.');
        }
        printf("\n");
    }
    printf("Legenda: X = ocupada; . = livre\n");
}

int contar_ocupadas(int mapa[FILEIRAS][COLUNAS]){

    int total = 0;
    for(int i = 0; i < FILEIRAS; i++){
        for(int j = 0; j < COLUNAS; j++){
            if(mapa[i][j] == VAGA_OCUPADA){
                total++;
            }
        }
    }
    return total;
}

void exibir_menu(void){

    printf("\n=== Sistema de Gestao de Estacionamento ===\n");
    printf("1 - Cadastrar veiculo\n");
    printf("2 - Consultar veiculo por identificador\n");
    printf("3 - Atualizar descricao ou categoria\n");
    printf("4 - Mover veiculo para outra vaga\n");
    printf("5 - Remover veiculo\n");
    printf("6 - Listar veiculos cadastrados\n");
    printf("7 - Exibir mapa de vagas\n");
    printf("8 - Exibir relatorio da ocupacao\n");
    printf("0 - Encerrar\n");
    printf("Opcao: ");
}

int ler_inteiro(int *valor){

    char linha[64];
    char *fim;

    if(fgets(linha, sizeof linha, stdin) == NULL){
        return 0;
    }

    long n = strtol(linha, &fim, 10);

    if(fim == linha){
        return 0;
    }while(*fim == ' ' || *fim == '\t' || *fim == '\n' || *fim == '\r'){
        fim++;
    }
    if(*fim != '\0' || n < -1000000 || n > 1000000){
        return 0;
    }

    *valor = (int)n;
    return 1;
}

int frota_iniciar(Frota *f){

    f->dados = malloc(CAPACIDADE_INICIAL * sizeof(Veiculo));
    if(f->dados == NULL){
        f->usados = 0;
        f->capacidade = 0;
        return 0;
    }
    f->usados = 0;
    f->capacidade = CAPACIDADE_INICIAL;
    return 1;
}

int frota_garantir_espaco(Frota *f){

    if(f->usados < f->capacidade){
        return 1;
    }

    int nova_cap = f->capacidade * 2;

    Veiculo *novo = realloc(f->dados, nova_cap * sizeof(Veiculo));
    if(novo == NULL){
        return 0;
    }
    f->dados = novo;
    f->capacidade = nova_cap;
    return 1;
}

void frota_liberar(Frota *f){

    free(f->dados);
    f->dados = NULL;
    f->usados = 0;
    f->capacidade = 0;
}
