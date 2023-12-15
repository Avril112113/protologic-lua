#pragma once


void asyncify_start_unwind(void* buf) __attribute__((__import_module__("asyncify"), __import_name__("start_unwind")));
void asyncify_stop_unwind() __attribute__((__import_module__("asyncify"), __import_name__("stop_unwind")));
void asyncify_start_rewind(void* buf) __attribute__((__import_module__("asyncify"), __import_name__("start_rewind")));
void asyncify_stop_rewind() __attribute__((__import_module__("asyncify"), __import_name__("stop_rewind")));
// int32_t asyncify_get_state() __attribute__((__import_module__("asyncify"), __import_name__("get_state")));
