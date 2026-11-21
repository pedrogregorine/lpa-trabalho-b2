#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "estacionamento.h"

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

int posicao_valida(int fileira, int coluna){

    return fileira >= 1 && fileira <= FILEIRAS &&
           coluna  >= 1 && coluna  <= COLUNAS;
}

int normalizar_id(const char *entrada, char saida[TAM_ID + 1]){

    if(strlen(entrada) != TAM_ID){
        return 0;
    }
    for(int i = 0; i < TAM_ID; i++){
        char c = entrada[i];
        if(c >= 'a' && c <= 'z'){
            c = (char)(c - 'a' + 'A');
        }
        if(!((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))){
            return 0;
        }
        saida[i] = c;
    }
    saida[TAM_ID] = '\0';
    return 1;
}

int descricao_valida(const char *descricao){

    size_t n = strlen(descricao);
    if(n == 0 || n > MAX_DESCRICAO){
        return 0;
    }
    for(size_t i = 0; i < n; i++){
        if(descricao[i] != ' ' && descricao[i] != '\t'){
            return 1;
        }
    }
    return 0;
}

int buscar_veiculo(const Frota *f, const char *id){

    for(int i = 0; i < f->usados; i++){
        if(strcmp(f->dados[i].id, id) == 0){
            return i;
        }
    }
    return -1;
}

StatusCadastro cadastrar_veiculo(Frota *f, int mapa[FILEIRAS][COLUNAS],
                                 const char *id_entrada, const char *descricao,
                                 int categoria, int fileira, int coluna){
    char id[TAM_ID + 1];

    if(f->usados >= TOTAL_VAGAS){
        return CAD_LOTADO;
    }
    if(!normalizar_id(id_entrada, id)){
        return CAD_ID_INVALIDO;
    }
    if(buscar_veiculo(f, id) != -1){
        return CAD_ID_DUPLICADO;
    }
    if(!descricao_valida(descricao)){
        return CAD_DESC_INVALIDA;
    }
    if(categoria != CATEGORIA_CARRO && categoria != CATEGORIA_MOTO){
        return CAD_CATEGORIA_INVALIDA;
    }
    if(!posicao_valida(fileira, coluna)){
        return CAD_POSICAO_INVALIDA;
    }
    if(mapa[fileira - 1][coluna - 1] == VAGA_OCUPADA){
        return CAD_VAGA_OCUPADA;
    }

    if(!frota_garantir_espaco(f)){
        return CAD_SEM_MEMORIA;
    }

    Veiculo *novo = &f->dados[f->usados];
    strcpy(novo->id, id);
    strcpy(novo->descricao, descricao);
    novo->categoria = categoria;
    novo->fileira = fileira;
    novo->coluna = coluna;
    f->usados++;
    mapa[fileira - 1][coluna - 1] = VAGA_OCUPADA;

    return CAD_OK;
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

int ler_linha(char *buf, size_t tam){

    if(fgets(buf, (int)tam, stdin) == NULL){
        return 0;
    }

    size_t n = strlen(buf);
    if(n > 0 && buf[n - 1] == '\n'){
        buf[n - 1] = '\0';
        if(n > 1 && buf[n - 2] == '\r'){
            buf[n - 2] = '\0';
        }
        return 1;
    }

    int c, sobrou = 0;
    while((c = getchar()) != '\n' && c != EOF){
        sobrou = 1;
    }
    return sobrou ? -1 : 1;
}

int ler_inteiro(int *valor){

    char linha[64];
    char *fim;

    if(ler_linha(linha, sizeof linha) != 1){
        return 0;
    }

    long n = strtol(linha, &fim, 10);
    if(fim == linha){
        return 0;
    }while(*fim == ' ' || *fim == '\t'){
        fim++;
    }
    if(*fim != '\0' || n < -1000000 || n > 1000000){
        return 0;
    }

    *valor = (int)n;
    return 1;
}

static const char *nome_categoria(int categoria){

    return categoria == CATEGORIA_CARRO ? "Carro" : "Moto";
}

void menu_cadastrar(Frota *f, int mapa[FILEIRAS][COLUNAS]){

    char id[TAM_ID + 2];
    char idn[TAM_ID + 1];
    char desc[MAX_DESCRICAO + 2];
    int categoria, fileira, coluna;

    printf("Identificador (7 caracteres, A-Z e 0-9): ");
    if(ler_linha(id, sizeof id) != 1){
        printf("Entrada invalida ou longa demais. Cadastro cancelado.\n");
        return;
    }
    printf("Descricao (1 a 60 caracteres): ");
    if(ler_linha(desc, sizeof desc) != 1){
        printf("Entrada invalida ou longa demais. Cadastro cancelado.\n");
        return;
    }
    printf("Categoria (1 = carro, 2 = moto): ");
    if(!ler_inteiro(&categoria)){
        printf("Valor numerico invalido. Cadastro cancelado.\n");
        return;
    }
    printf("Fileira (1 a %d): ", FILEIRAS);
    if(!ler_inteiro(&fileira)){
        printf("Valor numerico invalido. Cadastro cancelado.\n");
        return;
    }
    printf("Coluna (1 a %d): ", COLUNAS);
    if(!ler_inteiro(&coluna)){
        printf("Valor numerico invalido. Cadastro cancelado.\n");
        return;
    }

    switch(cadastrar_veiculo(f, mapa, id, desc, categoria, fileira, coluna)){
        case CAD_OK:
            normalizar_id(id, idn);
            printf("Veiculo %s cadastrado na vaga F%d/C%d.\n",
                   idn, fileira, coluna);
            break;
        case CAD_LOTADO:
            printf("Estacionamento lotado (%d vagas). Cadastro recusado.\n",
                   TOTAL_VAGAS);
            break;
        case CAD_ID_INVALIDO:
            printf("Identificador invalido: use exatamente 7 caracteres "
                   "A-Z ou 0-9, sem espacos ou simbolos.\n");
            break;
        case CAD_ID_DUPLICADO:
            printf("Ja existe um veiculo com esse identificador.\n");
            break;
        case CAD_DESC_INVALIDA:
            printf("Descricao invalida: deve ter de 1 a %d caracteres.\n",
                   MAX_DESCRICAO);
            break;
        case CAD_CATEGORIA_INVALIDA:
            printf("Categoria invalida: use 1 (carro) ou 2 (moto).\n");
            break;
        case CAD_POSICAO_INVALIDA:
            printf("Posicao invalida: fileira de 1 a %d e coluna de 1 a %d.\n",
                   FILEIRAS, COLUNAS);
            break;
        case CAD_VAGA_OCUPADA:
            printf("A vaga F%d/C%d ja esta ocupada.\n", fileira, coluna);
            break;
        case CAD_SEM_MEMORIA:
            printf("Erro: memoria insuficiente. Cadastro cancelado.\n");
            break;
    }
}

void menu_consultar(const Frota *f){

    char entrada[TAM_ID + 2];
    char id[TAM_ID + 1];

    printf("Identificador: ");
    if(ler_linha(entrada, sizeof entrada) != 1 ||
        !normalizar_id(entrada, id)){
        printf("Identificador invalido; veiculo nao localizado.\n");
        return;
    }

    int i = buscar_veiculo(f, id);
    if(i < 0){
        printf("Veiculo %s nao localizado.\n", id);
        return;
    }

    const Veiculo *v = &f->dados[i];
    printf("Identificador: %s\n", v->id);
    printf("Descricao:     %s\n", v->descricao);
    printf("Categoria:     %s\n", nome_categoria(v->categoria));
    printf("Posicao:       F%d/C%d\n", v->fileira, v->coluna);
}

StatusAtualizacao atualizar_veiculo(Frota *f, const char *id_entrada,
                                    const char *nova_desc, int nova_cat){

    char id[TAM_ID + 1];

    if(!normalizar_id(id_entrada, id)){
        return ATU_ID_INVALIDO;
    }
    int pos = buscar_veiculo(f, id);
    if(pos < 0){
        return ATU_NAO_ENCONTRADO;
    }
    if(nova_desc != NULL && !descricao_valida(nova_desc)){
        return ATU_DESC_INVALIDA;
    }
    if(nova_cat != 0 && nova_cat != CATEGORIA_CARRO && nova_cat != CATEGORIA_MOTO){
        return ATU_CATEGORIA_INVALIDA;
    }
    if(nova_desc == NULL && nova_cat == 0){
        return ATU_NADA_A_ALTERAR;
    }

    Veiculo *v = &f->dados[pos];
    if(nova_desc != NULL){
        strcpy(v->descricao, nova_desc);
    }
    if(nova_cat != 0){
        v->categoria = nova_cat;
    }
    return ATU_OK;
}

void menu_atualizar(Frota *f){

    char entrada[TAM_ID + 2];
    char id[TAM_ID + 1];
    char desc[MAX_DESCRICAO + 2];
    char linha[64];
    char *fim;
    const char *nova_desc = NULL;
    int nova_cat = 0;

    printf("Identificador: ");
    if(ler_linha(entrada, sizeof entrada) != 1 || !normalizar_id(entrada, id)){
        printf("Identificador invalido; veiculo nao localizado.\n");
        return;
    }

    int pos = buscar_veiculo(f, id);
    if(pos < 0){
        printf("Veiculo %s nao localizado.\n", id);
        return;
    }

    printf("Descricao atual: %s\n", f->dados[pos].descricao);
    printf("Categoria atual: %s\n", nome_categoria(f->dados[pos].categoria));

    printf("Nova descricao (Enter para manter): ");
    if(ler_linha(desc, sizeof desc) != 1){
        printf("Entrada invalida ou longa demais. Atualizacao cancelada.\n");
        return;
    }
    if(desc[0] != '\0'){
        nova_desc = desc;
    }

    printf("Nova categoria (1 = carro, 2 = moto, Enter para manter): ");
    if(ler_linha(linha, sizeof linha) != 1){
        printf("Entrada invalida. Atualizacao cancelada.\n");
        return;
    }
    if(linha[0] != '\0'){
        long n = strtol(linha, &fim, 10);
        while(*fim == ' ' || *fim == '\t'){
            fim++;
        }
        if(fim == linha || *fim != '\0' || n < 1 || n > 2){
            printf("Categoria invalida: use 1 (carro) ou 2 (moto). Atualizacao cancelada.\n");
            return;
        }
        nova_cat = (int)n;
    }

    switch(atualizar_veiculo(f, id, nova_desc, nova_cat)){
        case ATU_OK:
            printf("Veiculo %s atualizado.\n", id);
            printf("Descricao: %s\n", f->dados[pos].descricao);
            printf("Categoria: %s\n", nome_categoria(f->dados[pos].categoria));
            break;
        case ATU_NADA_A_ALTERAR:
            printf("Nada foi alterado.\n");
            break;
        case ATU_DESC_INVALIDA:
            printf("Descricao invalida: deve ter de 1 a %d caracteres.\n", MAX_DESCRICAO);
            break;
        case ATU_CATEGORIA_INVALIDA:
            printf("Categoria invalida: use 1 (carro) ou 2 (moto).\n");
            break;
        case ATU_ID_INVALIDO:
        case ATU_NAO_ENCONTRADO:
            printf("Veiculo nao localizado.\n");
            break;
    }
}
