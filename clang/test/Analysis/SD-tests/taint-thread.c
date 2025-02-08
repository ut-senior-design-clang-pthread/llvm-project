// RUN: %clang_analyze_cc1 -Wno-strict-prototypes -Wno-error=implicit-int -verify  %s \
// RUN:   -analyzer-checker=core,optin.taint.GenericTaint -DPTHREAD_MODEL=1 \
// RUN:   -analyzer-checker=debug.ExprInspection -analyzer-config model-pthreads=true

// RUN: %clang_analyze_cc1 -Wno-strict-prototypes -Wno-error=implicit-int -verify  %s \
// RUN:   -analyzer-checker=core,optin.taint.GenericTaint -DNO_PTHREAD_MODEL=1 \
// RUN:   -analyzer-checker=debug.ExprInspection -analyzer-config model-pthreads=false

#define NULL ((void*) 0)
typedef unsigned long int pthread_t;
typedef struct __pthread_attr pthread_attr_t;
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);

char *strcat( char *dest, const char *src );
int scanf(const char*, ...);
int system(const char *command);

void *thread_func(void *arg) {
#ifdef PTHREAD_MODEL
    system( (char *) arg); // expected-warning {{Untrusted data is passed to a system call (CERT/STR02-C. Sanitize data passed to complex subsystems)}}
#endif
#ifdef NO_PTHREAD_MODEL 
    system( (char *) arg); // expected-no-diagnostics
#endif
    return NULL;
}

// Command Injection Vulnerability Example
void test(void) {
  char cmd[2048] = "/bin/cat ";
  char filename[1024];
  scanf (" %1023[^\n]", filename); // The attacker can inject a shell escape here
  strcat(cmd, filename);
  pthread_t p1;
  pthread_create(&p1, NULL, &thread_func, &cmd);
}

