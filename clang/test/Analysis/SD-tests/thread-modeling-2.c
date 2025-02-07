// RUN: %clang_analyze_cc1 -Wno-strict-prototypes -Wno-error=implicit-int -verify  %s \
// RUN:   -analyzer-checker=debug.ExprInspection \
// RUN:   -analyzer-checker=core \
// RUN:   -analyzer-checker=unix


#define NULL ((void*) 0)
enum bool {
	false,
	true
};


typedef unsigned long int pthread_t;
typedef struct __pthread_attr pthread_attr_t;

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);


int pthread_join(pthread_t thread, void **retval);

void clang_analyzer_checkInlined(enum bool);
void clang_analyzer_warnIfReached();

void* thread_function(void* arg){
	// should expect to fail the test at this line if you set the checkInlined to true
	clang_analyzer_checkInlined(true);		// expected-warning{{TRUE}}
    if(true){
      clang_analyzer_warnIfReached();      //expected-warning{{REACHABLE}}
    }
    else{
      clang_analyzer_warnIfReached();        // no-warning
    }
	return NULL;
}

int foo_no_pthread(void)
{
	// should expect to pass at this line
	clang_analyzer_checkInlined(true);		// expected-warning{{TRUE}}
  if(true){
    clang_analyzer_warnIfReached();      //expected-warning{{REACHABLE}}
  }
  else{
    clang_analyzer_warnIfReached();        // no-warning
  }
	return 0;
}

int main(){
	pthread_t p1;
	pthread_create(&p1, NULL, &thread_function, NULL);
	pthread_join(p1, NULL);
	foo_no_pthread();
	return 1;
}

