#ifndef ESTACIONAMENTO_H
#define ESTACIONAMENTO_H

#define FILEIRAS 4
#define COLUNAS 5
#define TOTAL_VAGAS (FILEIRAS * COLUNAS)

#define VAGA_LIVRE 0
#define VAGA_OCUPADA 1

void inicializar_mapa(int mapa[FILEIRAS][COLUNAS]);

void exibir_mapa(int mapa[FILEIRAS][COLUNAS]);

int contar_ocupadas(int mapa[FILEIRAS][COLUNAS]);

void exibir_menu(void);

int ler_inteiro(int *valor);

#endif
