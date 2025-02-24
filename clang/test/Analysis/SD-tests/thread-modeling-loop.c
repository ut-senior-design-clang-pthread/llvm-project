// RUN: %clang_analyze_cc1 -Wno-strict-prototypes -Wno-error=implicit-int -verify  %s \
// RUN:   -analyzer-checker=core,unix -DPTHREAD_MODEL=1 \
// RUN:   -analyzer-checker=debug.ExprInspection -analyzer-config model-pthreads=true
//
// RUN: %clang_analyze_cc1 -Wno-strict-prototypes -Wno-error=implicit-int -verify  %s \
// RUN:   -analyzer-checker=core,unix -DNO_PTHREAD_MODEL=1 \
// RUN:   -analyzer-checker=debug.ExprInspection -analyzer-config model-pthreads=false

void a();

void pthread_create(long *, struct b *, void *, void *);

void c() {

  while (1)

    a();

}

void d() { pthread_create(d, 0, c, a); }