#ifndef ESTACIONAMENTO_H
#define ESTACIONAMENTO_H

#include <stddef.h>

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

typedef enum{
    CAD_OK,
    CAD_LOTADO,
    CAD_ID_INVALIDO,
    CAD_ID_DUPLICADO,
    CAD_DESC_INVALIDA,
    CAD_CATEGORIA_INVALIDA,
    CAD_POSICAO_INVALIDA,
    CAD_VAGA_OCUPADA,
    CAD_SEM_MEMORIA
} StatusCadastro;

int frota_iniciar(Frota *f);

int frota_garantir_espaco(Frota *f);

void frota_liberar(Frota *f);

void inicializar_mapa(int mapa[FILEIRAS][COLUNAS]);
void exibir_mapa(int mapa[FILEIRAS][COLUNAS]);
int contar_ocupadas(int mapa[FILEIRAS][COLUNAS]);

int posicao_valida(int fileira, int coluna);

int normalizar_id(const char *entrada, char saida[TAM_ID + 1]);

int descricao_valida(const char *descricao);

int buscar_veiculo(const Frota *f, const char *id);

StatusCadastro cadastrar_veiculo(Frota *f, int mapa[FILEIRAS][COLUNAS],
                                 const char *id, const char *descricao,
                                 int categoria, int fileira, int coluna);

void exibir_menu(void);

int ler_linha(char *buf, size_t tam);

int ler_inteiro(int *valor);

void menu_cadastrar(Frota *f, int mapa[FILEIRAS][COLUNAS]);
void menu_consultar(const Frota *f);

typedef enum{
    ATU_OK,
    ATU_ID_INVALIDO,
    ATU_NAO_ENCONTRADO,
    ATU_DESC_INVALIDA,
    ATU_CATEGORIA_INVALIDA,
    ATU_NADA_A_ALTERAR
} StatusAtualizacao;

StatusAtualizacao atualizar_veiculo(Frota *f, const char *id_entrada,
                                    const char *nova_desc, int nova_cat);

void menu_atualizar(Frota *f);

typedef enum{
    MOV_OK,
    MOV_ID_INVALIDO,
    MOV_NAO_ENCONTRADO,
    MOV_POSICAO_INVALIDA,
    MOV_MESMA_VAGA,
    MOV_VAGA_OCUPADA
} StatusMovimento;

StatusMovimento mover_veiculo(Frota *f, int mapa[FILEIRAS][COLUNAS],
                              const char *id_entrada, int fileira, int coluna);

void menu_mover(Frota *f, int mapa[FILEIRAS][COLUNAS]);

typedef enum{
    REM_OK,
    REM_ID_INVALIDO,
    REM_NAO_ENCONTRADO
} StatusRemocao;

StatusRemocao remover_veiculo(Frota *f, int mapa[FILEIRAS][COLUNAS],
                              const char *id_entrada);

void menu_remover(Frota *f, int mapa[FILEIRAS][COLUNAS]);

#endif
