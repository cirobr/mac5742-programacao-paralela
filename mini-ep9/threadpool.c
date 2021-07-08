// Name: Ciro B Rosa
// NUSP: 2320769
// E-mail: ciro.rosa@alumni.usp.br

// In this mini EP we are going to implement the concept of a thread pool
// The idea is that instead of creating threads for each task,
// you create a pool o threads that waits for tasks to run,
// reducing the overhead of thread creation.

// Neste mini EP iremos implementar o conceito de thread pool (grupo the threads)
// A ideia consiste em criar um grupo de threads que esperam por tarefas a serem
// executadas ao invés de criar uma thread para cada tarefa,
// reduzindo a sobrecarga de criação de threads.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//------------------------------------------------------------------------------------

// Our demo program will run a series of dummies tasks that just call sleep
// but they could be complex tasks that download files, process video frames and so on.

// Nossa demonstração irá rodar uma série de tarefas de exemplo que apenas chamam sleep,
// mas elas poderiam ser tarefas mais complexas que baixam arquivos, processam quadros
// vídeos e assim por diante.

typedef struct _Task {
    unsigned int seconds;
} Task;

// We are going to use this function to run out task.
// Iremos usar essa função para rodar nossa tarefa.
void runTask(Task T);

typedef void *ThreadPool;

// Create a threadpool with numberOfThreads threads.
// Cria um grupo de threads com numberOfThreads threads.
ThreadPool newThreadPool(int numberOfThreads);

// Destroy um grupo de threads.
// It blocks until all threads returns.
// Destroi um grupo de threads.
// Ela apenas retorna quanto todas as threads retornarem.
void drainThreadPool(ThreadPool pool);

// Adds a task in the group of threads.
// Adiciona uma tarefa no grupo de threads.
void addTask(ThreadPool pool, Task t);

// Returns a task
// Retorna uma tarefa
Task getTask(ThreadPool pool);

//ciro
#define pthread_mutex_t pool;

int main(int argc, char ** argv) {
    int threads;
    if(argc < 2) {
        printf("%s", "number of threads: ");
        scanf("%d", &threads);
    } else {
        threads = atoi(argv[1]);
    }
    
    if(threads < 1) {
        puts("Must create at least one thread");
        exit(EXIT_FAILURE);
    }
    if(threads > 128) {
        puts("Do you really have that many number of hardware threads?");
        exit(EXIT_FAILURE);
    }

    ThreadPool pool = newThreadPool(threads);                   // ciro - aqui inicializa o pool e já começa a consumir da pilha!!!

    int ntasks = 9;
    int seconds[] = {1,1,1,1,1,1,1};  //{1,4,5,8,7,2,6};
    for(int i = 0; i < ntasks; i++) {
        // adds taks with different sleep durations
        printf("Adding %dth task to sleep %d\n", i, seconds[i%7]);
        addTask(pool, (Task) {.seconds = seconds[i%7]});        // ciro - porém só aqui a pilha é incrementada !!!
    }

    drainThreadPool(pool);
    return 0;
}

//------------------------------------------------------------------------------------
// Now it's time to code! But I will help a bit.
// Sections with ... means that you need to code it!
// Agora é hora de programar! Mas eu irei ajudar um pouco.
// Seções com ... significam que você precisa programar elas.

void runTask(Task T) {
    // our task just calls sleep
    // nossa tarefa apenas chama sleep
    sleep(T.seconds);
}

typedef struct _privateThreadPool {
    // thread data
    pthread_t      *threads;
    int             nthread;
    // task data
    // implementando estrutura de pilha
    int             top;            // topo da pilha
    int             capacity;       // capacidade da pilha
    int             stackInit;      // fila inicializada[0;1]
    int             *pElem;         // elemento da pilha
} privateThreadPool;

// Our thread runs tasks until it does not have tasks anymore.
// Nossa thread roda tarefas até que não tenha mais nenhuma.
void *thread(void * pool) {
    // This is a hack to use upper bits of a pointer to hold data
    // For 64 bit pointers, usually only 48 are used.
    // So we can fit a short int into it ;)
    // Esse é um hack para usar os bit superiores de um ponteiro para guardar
    // dados. Para um ponteiro de 64 bits, geralmente apenas 48 estão em uso.
    // Então podemos colocar um short int nele ;)

    //sleep(1);                   //ciro - solução mágica, agora a fila é criada antes de ser consumida

    long id = (long)pool >> 48;
    pool = (void*)((long)pool-(id<<48));
    //privateThreadPool *p = pool;
    while(1) {
        Task t = getTask(pool);
        if(t.seconds == 0) {
            break;
        }
        printf("Thread %ld Running task sleep %us\n", id, t.seconds);
        fflush(stdout);
        runTask(t);
    }
    printf("Thread %ld exiting\n", id);
    return NULL;
}

// max number os tasks in the scheduler before addTask blocks.
// máximo de taredas no escalonador antwes de addTask bloquear.
#define MAXTASKS 20

ThreadPool newThreadPool(int numberOfThreads) {
    privateThreadPool *p = malloc(sizeof(privateThreadPool));

    // Prepare task data structures
    // Prepara a estrutura de dados relacionada as tarefas
    // ...
    // Criar pilha
    p->top = 0;
    p->capacity = MAXTASKS;
    p->stackInit = 0;
    p->pElem = (int*) malloc(MAXTASKS * sizeof(int));

    // Creates and starts the threads
    p->threads = malloc(sizeof(pthread_t)*numberOfThreads);
    p->nthread = numberOfThreads;

    for(int i = 0; i < numberOfThreads; i++) {
        pthread_create(p->threads+i, NULL, thread, (void*)((long)p + ((long)i<<48)));
    }

    return p;
}

void drainThreadPool(ThreadPool pool) {
    privateThreadPool *p = pool;
    // We add ending tasks so all threads returns.
    // Nós adicionamos tarefas que encerram as threads
    // para que todas a threads retornem.
    for(int i = 0; i < p->nthread; i++) {
        // ...
        // dúvida sobre o código a colocar aqui. aparentemente não precisa
        // https://stackoverflow.com/questions/38793807/how-to-properly-free-array-of-pthread-t-in-c
    }
    for(int i = 0; i < p->nthread; i++) {
        pthread_join(p->threads[i], NULL);
    }
    // free stuff you've allocated
    // de free no que você alocou
    free(p->threads);
    free(p);
}


void addTask(ThreadPool pool, Task t) {
    privateThreadPool *p = pool;
    // Adds a task to the task queue in the thread pool
    // If the queue is full, it must block.
    // Adiciona uma tarefa na fila de tarefas do grupo de threads
    // Se a fila estiver cheia, ele deve bloquear.
    // ...

    if(p->top == 0)
    {
        pthread_mutex_unlock(pool);
    }

    if(p->top == p->capacity)
    {
        pthread_mutex_lock(pool);
    }

    else
    {
        pthread_mutex_lock(pool);
        p->pElem[p->top] = t.seconds;
        p->top++;
        p->stackInit = 1;
        pthread_mutex_unlock(pool);
    }

    return;
}

Task getTask(ThreadPool pool) {
    privateThreadPool *p = pool;
    Task t;
    // get the first task of the queue.
    // if the queue is empty this function must block.
    // obtem a primeira tarefa da fila.
    // se a fila estivar vazia, essa função bloqueia.
    // ...

    if(p->top == p->capacity)
    {
        pthread_mutex_unlock(pool);
    }

    if(p->top > 0)
    {
        pthread_mutex_lock(pool);
        t.seconds = p->pElem[p->top];
        p->top--;
        pthread_mutex_unlock(pool);
    }

    else
    {
        pthread_mutex_lock(pool);
        if(p->stackInit == 1) t.seconds = 0;
    }

    // return the task
    return t;
}
