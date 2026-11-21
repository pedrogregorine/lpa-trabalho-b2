#include <stdio.h>
#include "estacionamento.h"

int main(void){

    int mapa[FILEIRAS][COLUNAS];
    Frota frota;
    int opcao;

    inicializar_mapa(mapa);
    if(!frota_iniciar(&frota)){
        printf("Erro: memoria insuficiente para iniciar.\n");
        return 1;
    }

    do{
        exibir_menu();

        if(!ler_inteiro(&opcao)){
            if(feof(stdin)){
                printf("\nFim da entrada. Encerrando.\n");
                opcao = 0;
            }else{
                printf("Entrada invalida. Digite um numero do menu.\n");
                opcao = -1;
            }
            continue;
        }

        switch(opcao){
            case 1:
                menu_cadastrar(&frota, mapa);
                break;
            case 2:
                menu_consultar(&frota);
                break;
            case 3: case 4: case 5: case 6:
                printf("Opcao %d ainda nao implementada.\n", opcao);
                break;
            case 7:
                exibir_mapa(mapa);
                break;
            case 8:
                printf("Vagas ocupadas: %d de %d\n",
                       contar_ocupadas(mapa), TOTAL_VAGAS);
                break;
            case 0:
                printf("Encerrando.\n");
                break;
            default:
                printf("Opcao inexistente.\n");
        }
    }while(opcao != 0);

    frota_liberar(&frota);
    return 0;
}
