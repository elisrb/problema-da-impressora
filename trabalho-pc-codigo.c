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

// VARIÁVEIS GLOBAIS

// funções principais das threads
void * aluno(void *arg);
void * tecnico(void *arg);

// funções auxiliares
void prepara_impressao(int i);
void usa_impressora(int i, int imp);
void manutencao(int i);

// mecanismos de sincronização
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // um único lock foi utilizada
pthread_cond_t pode_imprimir = PTHREAD_COND_INITIALIZER; // variável de condição de liberação da impressora
pthread_cond_t fim_manutencao = PTHREAD_COND_INITIALIZER; // variável de condição de fim da manutenção

// variáveis de controle (compartilhadas)
int cont = 0; // quantas impressoras estão sendo utilizadas no momento
int em_manutencao = 0; // 0 = impressoras estão lives, 1 = impressoras estão em manutenção
int disponivel[IMPRESSORAS]; // quais impressoras estão sendo utilizadas (0 = disponível, 1 = em uso)

int main() {
    srand(time(NULL)); // gerador de números aleatórios

    // inicializa todas as impressoras como disponíveis
    for (int imp = 0; imp < IMPRESSORAS; imp++) disponivel[imp] = 0;

    pthread_t a[ALUNOS]; // threads de alunos
    pthread_t t; // thread do técnico

	int i; // contador dos loops
    int *id; // id das threads

    // cria uma thread para cada aluno
    for (i = 0; i < ALUNOS; i++) {
        id = (int *) malloc(sizeof(int));
        *id = i;
        pthread_create(&a[i], NULL, aluno, (void *) (id));
	}

    // cria athread do técnico
    i++;
    id = (int *) malloc(sizeof(int));
    *id = i;
    pthread_create(&t, NULL, tecnico, (void *) (id));

    // join com a thread do técnico (o código não encerra)
	pthread_join(t,NULL);
	return 0;
}

// FUNÇÃO DO ALUNO
void* aluno(void *arg) {
	int i = *((int *) arg);

	while(1) { // repete para sempre
        printf("Aluno %d entrou na sala\n", i);

        prepara_impressao(i); // prepara o documento para impressão (região não crítica)

        pthread_mutex_lock(&mutex);
            while(em_manutencao == 1){ // enquanto estiver em manutenção
                printf("Aluno %d está esperando a manutenção acabar...\n", i);
                pthread_cond_wait(&fim_manutencao, &mutex); // aguarda o sinal de liberação de uma impressora
            }

            while(cont == IMPRESSORAS){ // enquanto não há impressoras livres
                printf("Aluno %d está esperando para imprimir...\n", i);
                pthread_cond_wait(&pode_imprimir, &mutex); // aguarda o sinal de liberação de uma impressora
            }
            cont++; // essa thread está usando as impressoras agora
            printf("Aluno %d vai imprimir agora\n", i);

            // encontra a primeira impressora disponível e usa ela
            int imp;
            for(imp = 0; imp < IMPRESSORAS; imp++){
                if(disponivel[imp] == 0){
                    disponivel[imp] = 1;
                    break;
                }
            }
        pthread_mutex_unlock(&mutex);

        usa_impressora(i, imp+1); // imprime o documento

        // libera a impressora
        pthread_mutex_lock(&mutex);
            cont--;
            disponivel[imp] = 0;
            printf("Aluno %d liberou a impressora %d\n", i, imp+1);
        pthread_mutex_unlock(&mutex);
        
        pthread_cond_signal(&pode_imprimir); // sinaliza para o próximo aluno que uma impressora foi liberada
        printf("Aluno %d saiu da sala\n", i);
        sleep(1);
	}
    pthread_exit(0); // encerra (nesse caso, as threads nunca serão encerradas pois estão em loop infinito)
}

// FUNÇÃO DO TÉCNICO
void* tecnico(void *arg) {
	int i = *((int *) arg);

	while(1) { // repete para sempre
        sleep(TEMPO); // espera o tempo necessário entre uma manutenção e outra
        printf("Técnico entrou na sala!\n");

        pthread_mutex_lock(&mutex);
            em_manutencao = 1; // impede que mais alunos comecem a imprimir a partir de agora

            // espera os alunos que estão imprimindo terminarem
            // a cada vez que uma impressora é liberada, ele checa se ainda há alguma em uso
            while(cont != 0){
                printf("Técnico está esperando todos terminarem...\n");
                pthread_cond_wait(&pode_imprimir, &mutex);
            }
        pthread_mutex_unlock(&mutex);

        manutencao(i); // faz a manutenção nas impressoras

        // encerra a manutenção
        pthread_mutex_lock(&mutex);
            em_manutencao = 0;
        pthread_mutex_unlock(&mutex);
        
        // avisa que a manutenção acabou para uma quantidade de alunos igual ao número de impressoras
        pthread_cond_broadcast(&fim_manutencao);
        printf("Técnico saiu da sala!\n");
	}
    pthread_exit(0); // encerra (nesse caso, as threads nunca serão encerradas pois estão em loop infinito)
}

// FUNÇÕES AUXILIARES (simulam a execução de tarefas nas threads)

void prepara_impressao(int i) {
	printf("Aluno %d está preparando um documento para impressão\n", i);
	sleep(rand() % 5);
}

void usa_impressora(int i, int imp) {
	printf("Aluno %d está usando a impressora %d\n", i, imp);
	sleep(rand() % 5);
}

void manutencao(int i) {
	printf("Técnico está fazendo manutenção nas impressoras!\n");
	sleep(rand() % 5);
}
