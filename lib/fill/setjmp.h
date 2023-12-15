// Custom implementation of setjmp/longjmp
// WASM doesn't support setjmp/longjmp.
// WASM also doesn't support exceptions, consequently WASI doesn't support them too, which could be used to implement setjmp/longjmp.
// Binaryen asyncify provides enough for us to implement setjmp/longjmp, this is based on https://github.com/kripken/talks/blob/master/jmp.c 


#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <asyncify.h>


#define __SETJMP_DBG_PRINTF false


#define ASYNCIFY_BUFFER_SIZE 16384
typedef struct __jmp_buf {
	void* top;
	void* end;
	void* unwound;
	unsigned char buffer[ASYNCIFY_BUFFER_SIZE];
	int state;
	int status;
} __jmp_buf;

typedef void* jmp_buf[sizeof(__jmp_buf)];

int setjmp(jmp_buf env);
void longjmp_(jmp_buf env, int status);
// Alias `longjmp` as `longjmp_`, due to, some odd behaviour?
#define longjmp(env, status) longjmp_(env, status)

__jmp_buf* __get_active_jmp_buf();

#if __SETJMP_DBG_PRINTF
#define __RUN_JMP_SUPPORT__HANDLE() ({         \
	__jmp_buf* _env = __get_active_jmp_buf();  \
	if (_env == NULL) {                        \
		printf("exit. RUN_JMP_SUPPORT\n");     \
		break;                                 \
	}                                          \
	asyncify_stop_unwind();                    \
	printf("suspended/unwound. RUN_JMP_SUPPORT env=%p\n", _env); \
	switch (_env->state) {                     \
		case 0:                                \
			_env->unwound = _env->top;         \
			break;                             \
		case 1:                                \
			_env->top = _env->unwound;         \
			break;                             \
		default:                               \
			abort();                           \
	}                                          \
	printf("resuming/rewinding. RUN_JMP_SUPPORT env=%p state=%d status=%d\n", _env, _env->state, _env->status); \
	asyncify_start_rewind(_env);               \
})
#else
#define __RUN_JMP_SUPPORT__HANDLE() ({         \
	__jmp_buf* _env = __get_active_jmp_buf();  \
	if (_env == NULL) {                        \
		break;                                 \
	}                                          \
	asyncify_stop_unwind();                    \
	switch (_env->state) {                     \
		case 0:                                \
			_env->unwound = _env->top;         \
			break;                             \
		case 1:                                \
			_env->top = _env->unwound;         \
			break;                             \
		default:                               \
			abort();                           \
	}                                          \
	asyncify_start_rewind(_env);               \
})
#endif


#define RUN_JMP_SUPPORT_VOID(FUNC_CALL) ({         \
	while (1) {                                    \
		FUNC_CALL;                                 \
		__RUN_JMP_SUPPORT__HANDLE();               \
	}                                              \
})

#define RUN_JMP_SUPPORT_RETURN(RTYPE, FUNC_CALL) ({\
	RTYPE result;                                  \
	while (1) {                                    \
		result = FUNC_CALL;                        \
		__RUN_JMP_SUPPORT__HANDLE();               \
	}                                              \
	result;                                        \
})

#ifdef __cplusplus
}
#endif