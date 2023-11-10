#include <stdio.h>
#include "biblioteca.h"
#include <string.h>


void menu(){
    printf("1. Cadastrar tarefa\n");
    printf("2. Excluir tarefa\n");
    printf("3. Listar tarefas\n");
    printf("4. Alterar tarefas\n");
    printf("5. Filtrar tarefas por prioridade\n");
    printf("6. Filtrar tarefas por categoria\n");
    printf("7. Filtrar tarefas por status\n");
    printf("8. Filtrar tarefas por prioridade e categoria\n");
    printf("9. Filtrar tarefas por prioridade\n");
    printf("10. Filtrar tarefas por categoria\n");
    printf("11. Exportar tarefas por prioridade e categoria\n");
    printf("12. Sair\n");
};

int criarTarefa(struct estadoPrograma *state){
    if(state->tamanho == 100)
        return ERRO;
    struct tarefa novaTarefa;
    int p = 0; // valor da prioridade
    char c[100]; // vetor p/ categoria
    char d[300]; // vetor p/ descricao
    char s[15];
    do{
        printf("Prioridade da tarefa (1-10):\n");
        scanf("%d", &p);
        getchar();
    }while(p <= 0 || p > 10);
    printf("Descricao da tarefa (max 300 caracteres):\n");
    scanf("%299[^\n]%*c", &d);
    printf("Categoria (max 100 caracteres):\n");
    scanf("%99[^\n]%*c", &c);
    printf("Status da tarefa (completo/em andamento/nao iniciado):\n");
    scanf("%15[^\n]%*c", &s);
    novaTarefa.prioridade = p;
    strcpy(novaTarefa.categoria, c);
    strcpy(novaTarefa.descricao, d);
    strcpy(novaTarefa.status, s);
    state->memoria[state->tamanho] = novaTarefa;
    return OK;
}

int atribuirMemoria(struct estadoPrograma *ponteiroEstado){
    FILE *f = fopen("dados.bin", "rb");
    if(f == NULL){
        printf("ERRO:\n");
        printf("Nao foi posivel abrir o arquivo dados.bin");
        return ERRO;
    }
    // carrega o tamanho
    fread(&(ponteiroEstado->tamanho), sizeof(int), 1, f);
    // carrega a array de tarefas
    for(int i = 0; i < ponteiroEstado->tamanho; i++){
        fread(&(ponteiroEstado->memoria[i]), sizeof(struct tarefa), 1, f);
    }
    printf("Tarefas salvas carregadas com sucesso!\n");
    fclose(f);
    return OK;
}

int salvar(struct estadoPrograma state){
    FILE *f = fopen("dados.bin", "wb");
    if(f == NULL){
        printf("ERRO:\n");
        printf("Não foi possível abrir o arquivo dados.bin.\n");
        return ERRO;
    }
    // salva o tamanho no binario
    fwrite(&state.tamanho, sizeof(int), 1, f);
    // salva a array de structs
    for(int i = 0; i < state.tamanho; i++){
        fwrite(&state.memoria[i], sizeof(struct tarefa), 1, f);
    }
    fclose(f);
    printf("Arquivo atualizado com sucesso!\n");
    return OK;
}

int listarTarefas(struct estadoPrograma *state){
    if(state->tamanho == 0) // nao ha tarefas registradas
        return ERRO;
    struct tarefa *tarefas = state->memoria;
    printf("@@@@ TAREFAS @@@\n");
    for(int i = 0; i < state->tamanho; i++){
        printf("\n%d.\n", i+1);
        printf("Prioridade: %d\n", tarefas[i].prioridade);
        printf("Descricao: %s\n", tarefas[i].descricao);
        printf("Categoria: %s\n", tarefas[i].categoria);
        printf("Status: %s\n", tarefas[i].status);
    }
    return OK;
}

/* A funcao consiste em rearranjar a array. Ja que ela tem tamanho fixo (100), nao existem elementos
 * "null" ou vazios. O que o programa faz eh mandar o elemento a ser deletado para a direita do vetor, sendo
 * posicao = tamanho-1. Apos isso, diretamente no switch da main, o tamanho eh diminuido em 1. Assim, apesar do elemento
 * deletado ainda existir na array, ele nao eh computado. Quando o usuario criar outra tarefa, as tarefas que nao sao
 * computadas serao sobrescritas, e como o tamanho aumentara, serao lidas.*/
int deletarTarefa(struct estadoPrograma *state, int index){
    int novoIndex = index-1; // o input começa em 1 para melhor listagem de tarefas
    /* caso a tarefa seja a ultima, retorna ok pra nao dar erro no runtime (i+1 seria invalido)
     * como o alvo ja eh o ultimo item da array, entao faz sentido retornar OK e apenas diminuir
     * o tamanho na main*/
    if(novoIndex == state->tamanho-1){
        return OK;
    }
    else if(state->tamanho > 0){
        for(int i = novoIndex; i < state->tamanho - 1; i++){
            state->memoria[i] = state->memoria[i+1];
        }
        return OK;
    }
    else{
        return ERRO;
    }
}
int alterarTarefa(struct estadoPrograma *state){
    char alt;
    int inputalterar = 0;
    int resultado;
    printf("Digite a posicao na lista da tarefa a ser alterada (1-%d).\n", state->tamanho);
    resultado = scanf("%d", &inputalterar);
    while(resultado != 1 || inputalterar > state->tamanho || inputalterar < 0);
    printf("O que voce deseja alterar?(P/D/C/S)\n");
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
    scanf("%c",&alt);
    if(alt=='P'){
        printf("Digite a nova prioridade: \n");
        scanf("%d", &state->memoria[inputalterar-1].prioridade);
    }
    else if(alt=='C'){
        printf("Digite a nova categoria: ");
        scanf("%s", state->memoria[inputalterar-1].categoria);
    }
    else if(alt=='D'){
        printf("Digite a nova descricao: ");
        scanf("%s", state->memoria[inputalterar-1].descricao);
    }
    else if(alt=='S'){
        printf("Digite o novo status: ");
        scanf("%s", state->memoria[inputalterar-1].status);
    }
    else{
        printf("Erro ao alterar tarefa.\n");
    }
}
int filprior(struct estadoPrograma *state){
    int pr;
    int x=0;
    printf("Digite a prioridade desejada (1-10).\n");
    scanf("%d",&pr);
    for (int i = 0; i < state->tamanho; ++i) {
        if(pr==state->memoria[i].prioridade){
            x++;
            printf("\nTarefa %d.\n", x);
            printf("Prioridade: %d\n", state->memoria[i].prioridade);
            printf("Descricao: %s\n", state->memoria[i].descricao);
            printf("Categoria: %s\n", state->memoria[i].categoria);
            printf("Status: %s\n", state->memoria[i].status);
        }
    }
    if (x==0){
        printf("\nNao ha tarefas com essa prioridade\n");
    }
}
int filcat(struct estadoPrograma *state){
    char cat[100];
    int x=0;
    printf("Digite a categoria desejada.\n");
    scanf("%s",cat);
    for (int i = 0; i < state->tamanho; ++i) {
        if(strcmp(cat,state->memoria[i].categoria)==0){
            x++;
            printf("\nTarefa %d.\n", x);
            printf("Prioridade: %d\n", state->memoria[i].prioridade);
            printf("Descricao: %s\n", state->memoria[i].descricao);
            printf("Categoria: %s\n", state->memoria[i].categoria);
            printf("Status: %s\n", state->memoria[i].status);
        }
    }
    if (x==0){
        printf("\nNao ha tarefas com essa categoria\n");
    }
}
int filstatus(struct estadoPrograma *state){
    char s[15];
    int x=0;
    printf("Digite o status desejado.\n");
    scanf("%s",s);
    for (int i = 0; i < state->tamanho; ++i) {
        if(strcmp(s,state->memoria[i].status) ==0){
            x++;
            printf("\nTarefa %d.\n", x);
            printf("Prioridade: %d\n", state->memoria[i].prioridade);
            printf("Descricao: %s\n", state->memoria[i].descricao);
            printf("Categoria: %s\n", state->memoria[i].categoria);
            printf("Status: %s\n", state->memoria[i].status);

        }
    }
    if (x==0){
        printf("\nNao ha tarefas com esse status\n");
    }
}
int filpecat(struct estadoPrograma *state){
    int pr;
    char cat[100];
    int x=0;
    printf("Digite a categoria desejada.\n");
    scanf("%s",cat);
    printf("Digite a prioridade desejada.\n");
    scanf("%d",&pr);
    for (int i = 0; i < state->tamanho; ++i) {
        if(pr==state->memoria[i].prioridade && strcmp(cat,state->memoria[i].categoria)==0){
            x++;
            printf("\nTarefa %d.\n", x);
            printf("Prioridade: %d\n", state->memoria[i].prioridade);
            printf("Descricao: %s\n", state->memoria[i].descricao);
            printf("Categoria: %s\n", state->memoria[i].categoria);
            printf("Status: %s\n", state->memoria[i].status);
        }
    }
    if (x==0){
        printf("\nNao ha tarefas com essa descricao e categoria\n");
    }
}
void exprior(struct estadoPrograma *state){
    int pr;
    int x=0;
    printf("Digite a prioridade desejada (1-10).\n");
    scanf("%d",&pr);
    FILE *f=fopen("lista.txt","w");
    for (int i = 0; i < state->tamanho; ++i) {
        if(pr==state->memoria[i].prioridade){
            x++;
            fprintf(f,"\nTarefa %d.\n", x);
            fprintf(f,"Prioridade: %d\n", state->memoria[i].prioridade);
            fprintf(f,"Descricao: %s\n", state->memoria[i].descricao);
            fprintf(f,"Categoria: %s\n", state->memoria[i].categoria);
            fprintf(f,"Status: %s\n", state->memoria[i].status);
        }
    }
    if (x==0){
        printf("\nNao ha tarefas com essa prioridade\n");
    }
}
void expcat(struct estadoPrograma *state){
    char cat[100];
    int x=0;
    printf("Digite a categoria desejada.\n");
    scanf("%s",cat);
    FILE *f=fopen("lista.txt","w");
    for (int i = 0; i < state->tamanho; ++i) {
        if(strcmp(cat,state->memoria[i].categoria)==0){
            x++;
            fprintf(f,"\nTarefa %d.\n", x);
            fprintf(f,"Prioridade: %d\n", state->memoria[i].prioridade);
            fprintf(f,"Descricao: %s\n", state->memoria[i].descricao);
            fprintf(f,"Categoria: %s\n", state->memoria[i].categoria);
            fprintf(f,"Status: %s\n", state->memoria[i].status);
        }
    }
    if (x==0){
        printf("\nNao ha tarefas com essa categoria\n");
    }
}
void expriorecat(struct estadoPrograma *state){
    int pr;
    char cat[100];
    int x=0;
    printf("Digite a categoria desejada.\n");
    scanf("%s",cat);
    printf("Digite a prioridade desejada.\n");
    scanf("%d",&pr);
    FILE *f=fopen("lista.txt","w");
    for (int i = 0; i < state->tamanho; ++i) {
        if(pr==state->memoria[i].prioridade && strcmp(cat,state->memoria[i].categoria)==0){
            x++;
            fprintf(f,"\nTarefa %d.\n", x);
            fprintf(f,"Prioridade: %d\n", state->memoria[i].prioridade);
            fprintf(f,"Descricao: %s\n", state->memoria[i].descricao);
            fprintf(f,"Categoria: %s\n", state->memoria[i].categoria);
            fprintf(f,"Status: %s\n", state->memoria[i].status);
        }
    }
    if (x==0){
        printf("\nNao ha tarefas com essa descricao e categoria\n");
    }
}