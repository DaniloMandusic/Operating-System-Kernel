//
// Created by os on 10/13/24.
//

#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.hpp"
#include "../h/CCB.hpp"

void* operator new (size_t size){
    return mem_alloc(size);
}

void* operator new[] (size_t size){
    return mem_alloc(size);
}

void operator delete (void* memory) noexcept{
    mem_free(memory);
}

void operator delete[] (void* memory) noexcept{
    mem_free(memory);
}

Thread::Thread(void (*body)(void *), void *arg) {
    this->body = body;
    this->arg = arg;
}

Thread::~Thread() {
    delete this->myHandle;
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::start() {

    if(this->body){
        return thread_create(&myHandle, body, arg);
    } else {
        return thread_create(&myHandle, &wrapper, (void*) this);
    }

}

Thread::Thread() {
    arg = this;
    body = nullptr;
}

