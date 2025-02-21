//
// Created by os on 4/7/24.
//

#ifndef PROJECT_BASE_SYSCALL_C_HPP
#define PROJECT_BASE_SYSCALL_C_HPP

#include "../lib/hw.h"
#include "../h/CCB.hpp"

void* mem_alloc(size_t size);

int mem_free(void*);

//class CCB;
typedef CCB *thread_t;
int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg);
int thread_create_no_start(thread_t* handle, void(*start_routine)(void*), void* arg);
int thread_start(CCB* ccb);
int thread_exit();
void thread_dispatch();

char getc();
void putc(char c);

#endif //PROJECT_BASE_SYSCALL_C_HPP


