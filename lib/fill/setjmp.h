// Custom implementation of setjmp/longjmp
// WASM doesn't support setjmp/longjmp.
// WASM also doesn't support exceptions, consequently WASI doesn't support them too, which could be used to implement setjmp/longjmp.


#ifdef __cplusplus
extern "C" {
#endif

typedef void* jmp_buf;


void longjmp(jmp_buf env, int status);
int setjmp(jmp_buf env);

jmp_buf get_longjmp_env();
int get_longjmp_status();

void reset_longjmp();

#ifdef __cplusplus
}
#endif