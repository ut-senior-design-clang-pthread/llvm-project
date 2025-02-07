// RUN: %clang_analyze_cc1 -Wno-strict-prototypes -Wno-error=implicit-int -verify  %s \
// RUN:   -analyzer-checker=debug.ExprInspection

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

void* thread_function(void* arg){
	// should expect to fail the test at this line if you set the checkInlined to true
	clang_analyzer_checkInlined(true);		// expected-warning{{TRUE}}
	return NULL;
}

int ok()
{
	pthread_t p1;
	pthread_create(&p1, NULL, &thread_function, NULL);
	return 0;
}

