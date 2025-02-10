// RUN: %clang_analyze_cc1 -Wno-strict-prototypes -Wno-error=implicit-int -verify  %s \
// RUN:   -analyzer-checker=core,unix -DPTHREAD_MODEL=1 \
// RUN:   -analyzer-checker=debug.ExprInspection -analyzer-config model-pthreads=true
//
// RUN: %clang_analyze_cc1 -Wno-strict-prototypes -Wno-error=implicit-int -verify  %s \
// RUN:   -analyzer-checker=core,unix -DNO_PTHREAD_MODEL=1 \
// RUN:   -analyzer-checker=debug.ExprInspection -analyzer-config model-pthreads=false

#define NULL ((void*) 0)
typedef __typeof(sizeof(int)) size_t;
typedef unsigned long int pthread_t;
typedef struct __pthread_attr pthread_attr_t;

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);

void clang_analyzer_checkInlined(int);
void clang_analyzer_dump(int);

typedef struct _mystruct {
  int a;
  int b;
} mystruct, *pmystruct;

void *malloc(size_t sz);
void free (void* ptr);

static void* thread_function(void* arg)
{
   pmystruct ps = (pmystruct) arg;
   // should expect to fail the test at this line if you set the checkInlined to true
   int *ptr = (int *)arg;
   clang_analyzer_dump(*ptr);      // expected-warning-re{{reg_${{[[:digit:]]+}}<int Element{SymRegion{reg_${{[[:digit:]]+}}<void * arg>},0 S64b,int}}}
   free(arg);
   return NULL;
}

void create_worker( void *(*func)(void *), void * arg) {
  pthread_t p1;
  pthread_create(&p1, NULL, func, arg);
}

int mem[256]; 

int test()
{
  pmystruct ps = (pmystruct) malloc(sizeof(mystruct));
  ps->a = 1;
  ps->b = 2;

  // The static analyzer gives up on analyzing a code path for
  // iterations that exceed a limit.
  // See https://discourse.llvm.org/t/loop-handling-improvement-plans/80417
  //
  // To prove this to yourself, change 256 to 1 and rerun. Change
  // to a few different numbers and explore where the LIT test
  // starts to produce unexpected values.
  for (int i=0; i<256; i++) {
    mem[i] = 0;
  }
  create_worker(thread_function, ps);

  return 0;
}

