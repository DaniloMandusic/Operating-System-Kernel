//
// Created by os on 10/11/24.
//

//#include "../lib/mem.h"
#include "../lib/hw.h"
#include "../h/MemAllocator.hpp"

//void *operator new(uint64 n){
//    //return __mem_alloc(n);
//    return MemAllocator::mem_alloc(n);
//}
//
//void *operator new[](uint64 n){
//    //return __mem_alloc(n);
//    return MemAllocator::mem_alloc(n);
//}
//
//void operator delete (void *p) noexcept {
//    //__mem_free(p);
//    MemAllocator::mem_free(p);
//}
//
//void operator delete[] (void *p) noexcept {
//    //__mem_free(p);
//    MemAllocator::mem_free(p);
//}