/* Mini EP 5
 */

char * NAME = "Ciro B Rosa";
char * NUSP = "2320769";

/* 
 * To test the code, invoke it with the test argument
 * Para testar o código, invoque com o argumento test
 *
 * e.g.: ./a.out test
 * 
 * Divirtam-se ;)
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

// Prototype

int _epinit(int argc, char **argv);
int _run(void);
typedef void *obj;

// KMP helper functions

// creates a state machine for use on KMP
obj createState(char * string);
// clones it, cheaper than creating a new one
obj cloneState(const obj srcState);
// free a State object
void freeState(obj state);

// advance consumes a char
// and returns 1 if the search string was found, reseting its state
// return 0 otherwise
int advance(obj state, char cur, char next);
// return the length of the search string
int length(obj state);

int main(int argc, char **argv) {
	if(_epinit(argc, argv))
		return _run();
	return EXIT_FAILURE;
}


//******************************************************************************
// Your code starts here
//******************************************************************************

// Check this variable to get the number of threads to create
// Verfique essa variavel para obter o número de threads a criar
unsigned short int threads;

void *thread_advance(void *threadid, int ct, obj state, char cur, char next){
	long tid;
	tid = (long)threadid;

	// core to be executed in parallel
	if(advance(state, cur, next)){
		ct++;
	}
	
	pthread_exit(ct);
}

// Sequential base implementation, change it to use pthreads
// Implementação sequencial base, altere ela para ter usar pthreads
int task(char * data, long len, char * search) {
	obj st = createState(search);
	int count = 0;
	for(int i = 0; i < len - 1; i++) {
		// add pthread instructions here
		}
	}
	freeState(st);

	return count;
}	

//******************************************************************************
// Your code stops here
//******************************************************************************

int _test_mode = 0;

void * _files[] = {
	"medium.txt", "large.txt", "huge.txt", NULL
};

struct {
	int file;
	char *title;
	char *query;
	int test;
	int expected;
} _runs[] = {
	{.file = 0, .title="Query 1", .query = "like", .test = 1, .expected = 222},
	{.file = 0, .title="Query 2", .query = "izzy", .test = 0, .expected = 96},
	{.file = 1, .title="Query 3", .query = "s s", .test = 0, .expected = 9213},
	{.file = 1, .title="Query 4", .query = "low", .test = 0, .expected = 9033},
	{.file = 1, .title="Query 5", .query = "Psychology", .test = 0, .expected = 432},
	{.file = 2, .title="Query 6", .query = "GGCCGGG", .test = 0, .expected = 313590},
	{.file = -1},
};

// Returns a timestamp in milliseconds
// It is not thread safe
long _getTS();

int _epinit(int argc, char **argv) {
	if(argc >= 2 && strcmp(argv[1], "test") == 0)
		_test_mode = 1;
	return 1;
}

int _run(void) {
	char * buff = NULL;
	int fileInBuffer = -1;
	long buffLen = 0;
	long t0, tf;

	printf("MiniEP 5\nNome:%s\nNUSP:%s\n\n", NAME, NUSP);

	for(int i = 0; _runs[i].file != -1; i++) {
		if(_test_mode && _runs[i].test != 1) continue;
		if(fileInBuffer != _runs[i].file) {
			free(buff);
			FILE *f = fopen(_files[_runs[i].file], "rb");
			fseek(f, 0, SEEK_END);
			long fsize = ftell(f);
			fseek(f, 0, SEEK_SET); 

			buffLen = fsize + 2;
			buff = malloc(buffLen);
			fread(buff, 1, fsize, f);
			buff[fsize + 1] = '\0';

			fclose(f);
			fileInBuffer = _runs[i].file;
		}

		printf("Run: %s\n", _runs[i].title);
		printf("Threads\tRun\tTime(ms)\n");

		// For simplicity we will run with 1, 2, 4, and 8 threads.
		// Five times each

		for(int t = 1; t < 16; t = t * 2) {
			threads = t;
			for(int e = 1; e <= 5; e++) {
				t0 = _getTS();
                
                //thanks Guerrero
                fflush(stdout);

				// run in another processes for safety, attach to the correct pid for debbuging!
				pid_t pid = fork();
				if(pid == 0) {
					int res = task(buff, buffLen, _runs[i].query);
					if(res != _runs[i].expected) {
						printf("Invalid response from task on %s. Got %d, expected %d\n", _runs[i].title, res, _runs[i].expected);
						exit(EXIT_FAILURE);
					}
					exit(EXIT_SUCCESS);
				}
				int status;
				waitpid(pid, &status, 0);

				tf = _getTS();

				if(status != 0) {
					printf("Running %s, the %d-th run with %d thread(s) did not yield the correct response or died. Exiting.\n", _runs[i].title, e, t);
					exit(EXIT_FAILURE);
				}

				printf("%d\t%d\t%ld\n", t, e, tf-t0);
			}
		}
		puts("");
	}

	return 0;
}

long _getTS() {
	static struct timespec t;
	// good enough at the moment
	clock_gettime(CLOCK_MONOTONIC, &t); 

	return t.tv_sec * 1000 + t.tv_nsec/1000000;
}

struct _State {
	int *fsm;
	int s;
	int l;
	char *q;
};

// creates a state machine for use on KMP
obj createState(char * string) {
	int l = strlen(string);
	int s = 0;
	int *fsm = malloc(sizeof(int)*l);
	struct _State *st = malloc(sizeof(struct _State));
	st->fsm = fsm;
	st->s = s;
	st->l = l;
	st->q =string;
	
	// LPSA compute

	int len = 0;
	fsm[0] = 0;

	for(int i = 1; i < l;) {
		// keep growing
		if(string[i] == string[len]) {
			len++;
			fsm[i] = len;
			i++;
		} else {
			if(len != 0) {
				len = fsm[len - 1];
			} else {
				fsm[i] = 0;
				i++;
			}
		}
	}

	return st;
}
// clones it, cheaper than creating a new one
obj cloneState(const obj srcState) {
	struct _State *st = srcState, *nst = malloc(sizeof(struct _State));
	*nst = *st;
	nst->fsm = malloc(sizeof(int) * nst->l);
	for(int i = 0; i < nst->l; i++) nst->fsm[i] = st->fsm[i];
	return nst;
}

// free a State object
void freeState(obj state) {
	struct _State *st = state;
	free(st->fsm);
	free(st);
}

// advance consumes a char
// and returns 1 if the search string was found, reseting its state
// return 0 otherwise
int advance(obj state, char cur, char next) {
	struct _State *st = state;
	if(st->q[st->s] == cur) {
		st->s++;
	}
	if(st->s == st->l) {
		st->s = 0;
		return 1;
	} else if(st->q[st->s] != next) {
		st->s = st->s ? st->fsm[st->s-1] : 0;
	}
	return 0;
}

// return the length of the search string
int length(obj state) {
	return ((struct _State *)state)->l;
}
