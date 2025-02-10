// RUN: %clang_analyze_cc1 -Wno-strict-prototypes -Wno-error=implicit-int -verify  %s \
// RUN:   -analyzer-checker=core \
// RUN:   -analyzer-checker=debug.ExprInspection -analyzer-config model-pthreads=true

#define NULL ((void*) 0)
enum bool {
  false,
  true
};


typedef unsigned long int pthread_t;
typedef struct __pthread_attr pthread_attr_t;

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);

int pthread_join(pthread_t thread, void **retval);
void clang_analyzer_checkInlined(int);

void* worker1(void *arg){
  clang_analyzer_checkInlined(true);    // expected-warning{{TRUE}}
  int z = 0;
  if(z == 0){
    int c = 1 / z;  // expected-warning{{Division by zero}}
  }
  return NULL;
}

void* worker2(void *arg){
  clang_analyzer_checkInlined(true);    // expected-warning{{TRUE}}
  int y = 0;
  if(y == 0){
    int d = 1 / y;  // expected-warning{{Division by zero}}
  }
  return NULL;
}

int fee(){
  pthread_t t1, t2;
  pthread_create(&t1, NULL, worker1, NULL);
  pthread_create(&t2, NULL, worker2, NULL);
  return 0;
}