//
// Created by os on 4/7/24.
//
#include "../lib/hw.h"
#include "../h/syscall_c.hpp"
#include "../h/CCB.hpp"

void* mem_alloc(size_t size){
    uint64 code = 0x01;

    size_t sizeInBlocks;
    if (size%MEM_BLOCK_SIZE != 0){
        sizeInBlocks = (size/MEM_BLOCK_SIZE + 1) * MEM_BLOCK_SIZE;
    } else {
        sizeInBlocks = size;
    }

    __asm__ volatile ("mv a1, %0" : : "r" (sizeInBlocks));
    __asm__ volatile ("mv a0, %0" : : "r" (code));

    __asm__ volatile ("ecall");

    volatile uint64 ret;
    __asm__ volatile ("mv %0, a0": "=r" (ret));
    return (void*) ret;
}

int mem_free(void* addr){
    uint64 code = 0x02;
    __asm__ volatile ("mv a1, %0" : : "r" (addr));
    __asm__ volatile ("mv a0, %0" : : "r" (code));

    __asm__ volatile ("ecall");

    volatile uint64 ret;
    __asm__ volatile ("mv %0, a0": "=r" (ret));
    return (int) ret;
}

int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg){
    uint64 code = 0x11;

    __asm__ volatile ("mv a6, %[arg]" : : [arg] "r" (arg));
    __asm__ volatile ("mv a2, %[start_routine]" : : [start_routine] "r" (start_routine));
    __asm__ volatile ("mv a1, %[handle]" : : [handle] "r" (handle));
    __asm__ volatile ("mv a0, %[code]" : : [code] "r" (code));

    __asm__ volatile ("ecall");

    volatile uint64 ret;
    __asm__ volatile ("mv %0, a0": "=r" (ret));
    return (int) ret;

}

int thread_create_no_start (thread_t* handle, void(*start_routine)(void*), void* arg){
    uint64 code = 0x14;

    __asm__ volatile ("mv a6, %[arg]" : : [arg] "r" (arg));
    __asm__ volatile ("mv a2, %[start_routine]" : : [start_routine] "r" (start_routine));
    __asm__ volatile ("mv a1, %[handle]" : : [handle] "r" (handle));
    __asm__ volatile ("mv a0, %[code]" : : [code] "r" (code));

    __asm__ volatile ("ecall");

    volatile uint64 ret;
    __asm__ volatile ("mv %0, a0": "=r" (ret));
    return (int) ret;

}

int thread_start (CCB* ccb){
    uint64 code = 0x15;

    __asm__ volatile ("mv a1, %[ccb]" : : [ccb] "r" (ccb));
    __asm__ volatile ("mv a0, %[code]" : : [code] "r" (code));

    __asm__ volatile ("ecall");

    return 0;
}

int thread_exit(){
    uint64 code = 0x12;
    __asm__ volatile ("mv a0, %[code]" : : [code] "r" (code));

    __asm__ volatile ("ecall");

    volatile uint64 ret;
    __asm__ volatile ("mv %0, a0": "=r" (ret));
    return (int) ret;
}

void thread_dispatch(){
    uint64 code = 0x13;
    __asm__ volatile ("mv a0, %[code]" : : [code] "r" (code));

    __asm__ volatile ("ecall");
}

char getc() {
    uint64 code = 0x41;
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (code));
    __asm__ volatile ("ecall");

    volatile char ret;
    __asm__ volatile ("mv %0, a0": "=r" (ret));
    return (char) ret;
}

void putc(char c) {
    uint64 code = 0x42;
    __asm__ volatile ("mv a1, %[c]" : : [c] "r" (c));
    __asm__ volatile("mv a0, %[code]" : : [code] "r" (code));
    __asm__ volatile ("ecall");
}