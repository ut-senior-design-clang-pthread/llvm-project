// RUN: %clang_analyze_cc1 -Wno-strict-prototypes -Wno-error=implicit-int -verify %s \
// RUN:   -analyzer-checker=core \
// RUN:   -analyzer-checker=unix \
// RUN:   -analyzer-checker=debug.ExprInspection -analyzer-config model-pthreads=true

#define NULL ((void*) 0)

enum bool {
  false,
  true
};

typedef struct __pthread_attr pthread_attr_t;

typedef __typeof(sizeof(int)) size_t;

void free (void* ptr);

void *malloc(size_t sz);

void clang_analyzer_checkInlined(int);
void clang_analyzer_warnIfReached();
void clang_analyzer_printState();

typedef unsigned long int pthread_t;


int pthread_create(pthread_t *restrict thread,
       const pthread_attr_t *restrict attr,
       void *(*start_routine)(void*), void *restrict arg);


int pthread_join(pthread_t thread, void **retval);


void *worker(void *);
int foo(void);

void *worker(void *data)

{
  clang_analyzer_checkInlined(true);		// expected-warning{{TRUE}}
//  clang_analyzer_printState();

  int *pdata = (int *)data;
//  switch (*pdata) {
//  case 0:
//  case 2:
//      free(data);
//      break;
//  default:
//      break;
//  }

  return NULL;
}



int foo(void)
{
  pthread_t th1, th2;

  int *pdata1 = (int *)malloc(sizeof(int));
//  int *pdata2 = (int *)malloc(sizeof(int));


  *pdata1 = 0;
//  *pdata2 = 1;

//  clang_analyzer_printState();
  pthread_create(&th1, NULL, worker, pdata1);
//  pthread_create(&th2, NULL, worker, pdata2);

//
//  pthread_join(th1, NULL);
//  pthread_join(th2, NULL);

  return 0; // expected-warning {{Potential leak of memory pointed to by 'pdata1'}}
}

