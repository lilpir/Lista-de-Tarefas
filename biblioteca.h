//
// Created by unifwsilva on 12/09/2023.
//

#ifndef PROJETO_TAREFAS_BIBLIOTECA_H
#define PROJETO_TAREFAS_BIBLIOTECA_H

enum estadoLoop{
    FUNCIONANDO,
    FECHADO
};

enum gerenciarErro{
    OK,
    ERRO
};

struct tarefa{
    int prioridade;
    char descricao[300];
    char categoria[100];
    char status[15];
};

struct estadoPrograma{
    enum estadoLoop loop;
    struct tarefa memoria[100];
    int tamanho;
};

int atribuirMemoria(struct estadoPrograma *ponteiroEstado);
void menu();
int criarTarefa(struct estadoPrograma *state);
int salvar(struct estadoPrograma state);
int listarTarefas(struct estadoPrograma *state);
int deletarTarefa(struct estadoPrograma *state, int index);
//void input(char entrada[], char *saida);
int alterarTarefa(struct estadoPrograma *state);
int filprior(struct estadoPrograma *state);
int filcat(struct estadoPrograma *state);
int filstatus(struct estadoPrograma *state);
int filpecat(struct estadoPrograma *state);
void exprior(struct estadoPrograma *state);
void expcat(struct estadoPrograma *state);
void expriorecat(struct estadoPrograma *state);
#endif //PROJETO_TAREFAS_BIBLIOTECA_H
