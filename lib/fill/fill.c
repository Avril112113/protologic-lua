// This script fills missing methods.
// These are supposed to be supplied by wasi-libc, but idk :shrug:


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wasi/api.h>


FILE* tmpfile() {
	return NULL;
}

int system(char const* _Command) {
	return (_Command == NULL ? 0 : -1);
}

char* tmpnam(char *_Buffer) {
	return NULL;
}

static __wasi_timestamp_t start;
clock_t clock() {
	__wasi_timestamp_t now = 0;
    (void)__wasi_clock_time_get(__WASI_CLOCKID_MONOTONIC, 0, &now);
    return now - start;
}

__attribute__((constructor))
static void init(void) {
    (void)__wasi_clock_time_get(__WASI_CLOCKID_MONOTONIC, 0, &start);
}
