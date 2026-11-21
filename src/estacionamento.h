#ifndef ESTACIONAMENTO_H
#define ESTACIONAMENTO_H

#define FILEIRAS 4
#define COLUNAS 5
#define TOTAL_VAGAS (FILEIRAS * COLUNAS)

#define VAGA_LIVRE 0
#define VAGA_OCUPADA 1

#define TAM_ID 7
#define MAX_DESCRICAO 60
#define CAPACIDADE_INICIAL 2

#define CATEGORIA_CARRO 1
#define CATEGORIA_MOTO 2

typedef struct{
    char id[TAM_ID + 1];
    char descricao[MAX_DESCRICAO + 1];
    int categoria;
    int fileira;
    int coluna;
} Veiculo;

typedef struct{
    Veiculo *dados;
    int usados;
    int capacidade;
} Frota;

int frota_iniciar(Frota *f);

int frota_garantir_espaco(Frota *f);

void frota_liberar(Frota *f);

void inicializar_mapa(int mapa[FILEIRAS][COLUNAS]);

void exibir_mapa(int mapa[FILEIRAS][COLUNAS]);

int contar_ocupadas(int mapa[FILEIRAS][COLUNAS]);

void exibir_menu(void);

int ler_inteiro(int *valor);

#endif
