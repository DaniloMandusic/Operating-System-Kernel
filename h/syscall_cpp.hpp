//
// Created by os on 10/13/24.
//

#ifndef PROJECT_BASE1_SYSCALL_CPP_HPP
#define PROJECT_BASE1_SYSCALL_CPP_HPP

#include "../lib/hw.h"
#include "syscall_c.hpp"

void* operator new(size_t size);
void* operator new[](size_t size);

void operator delete(void*) noexcept;
void operator delete[](void*) noexcept;

class Thread {
public:
    Thread(void (*body)(void*), void* arg);
    virtual ~Thread();

    int start();

    static void dispatch();

protected:
    Thread();
    virtual void run(){};

private:
    thread_t myHandle;
    void (*body)(void*);
    void* arg;

    static void wrapper(void* thread){
        if(thread)
            ((Thread*)thread)->run();
    }
};

#endif //PROJECT_BASE1_SYSCALL_CPP_HPP
