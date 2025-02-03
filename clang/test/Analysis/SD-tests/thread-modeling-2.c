// RUN: %clang_analyze_cc1 -Wno-strict-prototypes -Wno-error=implicit-int -verify  %s \
// RUN: -I /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include \
// RUN:   -analyzer-checker=debug.ExprInspection

#define NULL ((void*) 0)
#include <stdbool.h>

typedef unsigned long int pthread_t;
typedef struct __pthread_attr pthread_attr_t;

int pthread_create(pthread_t *restrict thread,

       const pthread_attr_t *restrict attr,

       void *(*start_routine)(void*), void *restrict arg);



int pthread_join(pthread_t thread, void **retval);



void clang_analyzer_checkInlined(bool);

void* thread_function(void* arg){
	// should expect to fail the test at this line if you set the checkInlined to true
	clang_analyzer_checkInlined(false);		// no-warning
	return NULL;
}

int foo_no_pthread(void)
{
	// should expect to pass at this line
	clang_analyzer_checkInlined(true);		// expected-warning{{TRUE}}
	return 0;
}

int main(){
	pthread_t p1;
	pthread_create(&p1, NULL, &thread_function, NULL);
	pthread_join(p1, NULL);
	foo_no_pthread();
	return 1;
}

