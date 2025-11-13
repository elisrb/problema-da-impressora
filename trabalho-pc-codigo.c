/*
Desenvolvido por: Elis Rodrigues Borges - 231018875

PROBLEMA DA IMPRESSORA
Este código foi desenvolvido como parte do projeto da disciplina CIC0202 - Programação Concorrente,
Turma 01, durante o semestre 2025.2 na Universidade de Brasília. Ele implementa o algoritmo solução
do "Problema da Impressora" utilizando sincronização entre processos em C com a biblioteca pthreads.
*/

#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "pthread.h"

#define ALUNOS 10 // numero de alunos querendo usar as impressoras
#define IMPRESSORAS 2 // numero de impressoras disponíveis
#define TEMPO 15 // intervalo de tempo (em segundos) entre manutenções

int cont = 0;

void * aluno(void *arg);
void * tecnico(void *arg);
void prepara_impressao(int i);
void usa_impressora(int i, int imp);
void manutencao(int i);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int main() {
    srand(time(NULL));
	pthread_t a[ALUNOS];
    pthread_t t;

	int i;
    int *id;

    for (i = 0; i < ALUNOS ; i++) {
        id = (int *) malloc(sizeof(int));
        *id = i;
        pthread_create(&a[i], NULL, aluno, (void *) (id));
	}

    i++;
    id = (int *) malloc(sizeof(int));
    *id = i;
    pthread_create(&t, NULL, tecnico, (void *) (id));

	pthread_join(t,NULL);
	return 0;
}

void* aluno(void *arg) {
	int i = *((int *) arg);
	while(1) {
        prepara_impressao(i);
        usa_impressora(i, 1);
	}
    pthread_exit(0);
}

void* tecnico(void *arg) {
	int i = *((int *) arg);
	while(1) { // repete para sempre
        sleep(TEMPO); // espera o tempo necessário entre uma manutenção e outra
        manutencao(i); // faz a manutenção nas impressoras
	}
    pthread_exit(0); // encerra a thread (nesse caso)
}

void prepara_impressao(int i) {
	printf("Aluno %d está preparando um documento para impressão\n", i);
	sleep( rand() % 5);
    printf("Aluno %d está esperando para imprimir seu documento\n", i);
}

void usa_impressora(int i, int imp) {
	printf("Aluno %d está usando a impressora %d\n", i, imp);
	sleep(rand() % 5);
}

void manutencao(int i) {
	printf("Técnico está fazendo manutenção nas impressoras!\n");
	sleep(rand() % 5);
}
