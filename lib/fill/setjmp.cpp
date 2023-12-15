// Custom implementation of setjmp/longjmp
// WASM doesn't support setjmp/longjmp.
// WASM also doesn't support exceptions, consequently WASI doesn't support them too, which could be used to implement setjmp/longjmp.


#ifdef __cplusplus
extern "C" {
#endif

#include "setjmp.h"

#include <stdlib.h>
#include <stdio.h>
#include <asyncify.h>
#include <string.h>


static void jmp_buf_init(__jmp_buf* buf) {
	// buf->buffer = new unsigned char[ASYNCIFY_BUFFER_SIZE];
	buf->top = &buf->buffer[0];
	buf->end = &buf->buffer[ASYNCIFY_BUFFER_SIZE-1];
	buf->unwound = buf->top;
	buf->state = 0;
	buf->status = 0;
}


static __jmp_buf* _active_jmp_buf = NULL;

int setjmp(jmp_buf env) {
	__jmp_buf* _env = (__jmp_buf*)env;
	if (_active_jmp_buf == NULL) {
		// If we aren't doing initial rewind or a longjmp, this is a user call.
		// Setup new jmp_buf.
		jmp_buf_init(_env);
		_env->state = 0;
		_env->status = 0;
		_active_jmp_buf = _env;  // For initial rewind back here.
		#if __SETJMP_DBG_PRINTF
		printf("suspending/unwinding. setjmp:u env=%p\n", _env);
		#endif
		asyncify_start_unwind(_env);
		return 0;
	}
	// Not a user calling setjmp on a jmp_buf
	switch (_env->state)
	{
		case 0: {
			// From initial rewind
			asyncify_stop_rewind();
			#if __SETJMP_DBG_PRINTF
			printf("resumed/rewound. setjmp:0 env=%p\n", _env);
			#endif
			_active_jmp_buf = NULL;
			_env->state = 1;
			return 0;
		}
		case 1: {
			// From longjmp
			if (_env != _active_jmp_buf) {
				// If it's not a longjmp to us, we are just in the stack to another ahead, use the previous status.
				return _env->status;
			}
			asyncify_stop_rewind();
			#if __SETJMP_DBG_PRINTF
			printf("resumed/rewound. setjmp:1 env=%p\n", _env);
			#endif
			_active_jmp_buf = NULL;
			return _env->status;
		}
		default: {
			// Bad state
			asyncify_stop_rewind();
			printf("abort() setjmp:%d env=%p\n", _env->state, _env);
			// If this gets triggered, a possible buffer-overflow happend and corrupted the state value.
			abort();
		}
	}
}

static __jmp_buf tmp_buf;
void longjmp_(jmp_buf env, int status) {
	_active_jmp_buf = (__jmp_buf*)env;
	if (_active_jmp_buf->state != 1) {
		#if __SETJMP_DBG_PRINTF
		printf("longjmp with invalid env (state %d != 1)\n", _active_jmp_buf->state);
		#endif
		abort();
	}
	_active_jmp_buf->status = status;
	jmp_buf_init(&tmp_buf);
	#if __SETJMP_DBG_PRINTF
	printf("suspending/unwinding. longjmp env=%p\n", env);
	#endif
	asyncify_start_unwind(&tmp_buf);
}

__jmp_buf* __get_active_jmp_buf() {
	return _active_jmp_buf;
}


#ifdef __cplusplus
}
#endif