// Custom implementation of setjmp/longjmp
// WASM doesn't support setjmp/longjmp.
// WASM also doesn't support exceptions, consequently WASI doesn't support them too, which could be used to implement setjmp/longjmp.


#include "setjmp.h"

#include <stdlib.h>
#include <stdio.h>


static jmp_buf longjmp_env;
static int longjmp_status;

void longjmp(jmp_buf env, int status) {
	// printf("longjmp(%p, %d)", env, status);
	// fflush(stdout);
	longjmp_env = env;
	longjmp_status = status;
	abort();
}

int setjmp(jmp_buf env) {
	// printf("setjmp(%p)", env);
	// fflush(stdout);
	return 0;
}

jmp_buf get_longjmp_env() {
	return longjmp_env;
}
int get_longjmp_status() {
	return longjmp_status;
}

void reset_longjmp() {
	longjmp_env = NULL;
	longjmp_status = 0;
}
